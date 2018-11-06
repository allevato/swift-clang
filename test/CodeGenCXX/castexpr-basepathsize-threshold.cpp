// RUN: %clang_cc1 %s -emit-llvm -o -

// https://bugs.llvm.org/show_bug.cgi?id=38356
// We only check that we do not crash.

// ASAN increases stack usage, so we are hitting stack overflow before reaching
// recursive template instantiation limit.
// XFAIL: darwin && asan

template <typename a, a b(unsigned), int c, unsigned...>
struct d : d<a, b, c - 1> {};
template <typename a, a b(unsigned), unsigned... e>
struct d<a, b, 0, e...> {
  a f[0];
};
struct g {
  static g h(unsigned);
};
struct i {
  void j() const;
  // Current maximum depth of recursive template instantiation is 1024,
  // thus, this \/ threshold value is used here. BasePathSize in CastExpr might
  // not fit it, so we are testing that we do fit it.
  // If -ftemplate-depth= is provided, larger values (4096 and up) cause crashes
  // elsewhere.
  d<g, g::h, (1U << 10U) - 2U> f;
};
void i::j() const {
  const void *k{f.f};
  (void)k;
}
