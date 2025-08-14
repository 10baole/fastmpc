#include "fastmpc/flux/low/aby3/function/aby3_binary.h"

namespace fastmpc::flux::aby3 {

auto add_aa(FluxBuilder &builder, CipherValue x, CipherValue y) -> CipherValue {
  return CipherValue{
      .p0_x0 = builder.add(x.p0_x0, y.p0_x0),
      .p0_x1 = builder.add(x.p0_x1, y.p0_x1),
      .p1_x1 = builder.add(x.p1_x1, y.p1_x1),
      .p1_x2 = builder.add(x.p1_x2, y.p1_x2),
      .p2_x2 = builder.add(x.p2_x2, y.p2_x2),
      .p2_x0 = builder.add(x.p2_x0, y.p2_x0),
  };
}

auto xor_bb(FluxBuilder &builder, CipherValue x, CipherValue y) -> CipherValue {
  return CipherValue{
      .p0_x0 = builder._xor(x.p0_x0, y.p0_x0),
      .p0_x1 = builder._xor(x.p0_x1, y.p0_x1),
      .p1_x1 = builder._xor(x.p1_x1, y.p1_x1),
      .p1_x2 = builder._xor(x.p1_x2, y.p1_x2),
      .p2_x2 = builder._xor(x.p2_x2, y.p2_x2),
      .p2_x0 = builder._xor(x.p2_x0, y.p2_x0),
  };
}

namespace {

template <class Rng, class Add, class Mul, class Sub, class Cast>
auto mul_impl(Rng &&rng, Add &&add, Mul &&mul, Sub &&sub, Cast &&cast,
              CipherValue l, CipherValue r) {
  auto [p2_r0, p0_r0] = rng(2, 0);
  auto [p0_r1, p1_r1] = rng(0, 1);
  auto [p1_r2, p2_r2] = rng(1, 2);

  // z0 = x0y0 + x0y1 + x1y0 + r0 - r1
  // z1 = x1y1 + x1y2 + x2y1 + r1 - r2
  // z1 = x2y2 + x2y0 + x0y2 + r2 - r0
  auto func = [&](OpHandle x0, OpHandle y0, OpHandle x1, OpHandle y1,
                  OpHandle r0, OpHandle r1) {
    auto x0y0 = mul(x0, y0);
    auto x0y1 = mul(x0, y1);
    auto x1y0 = mul(x1, y0);
    auto r0_r1 = sub(r0, r1);
    return add(add(x0y0, x0y1), add(x1y0, r0_r1));
  };
  auto z0 = func(l.p0_x0, r.p0_x0, l.p0_x1, r.p0_x1, p0_r0, p0_r1);
  auto z1 = func(l.p1_x1, r.p1_x1, l.p1_x2, r.p1_x2, p1_r1, p1_r2);
  auto z2 = func(l.p2_x2, r.p2_x2, l.p2_x0, r.p2_x0, p2_r2, p2_r0);

  auto result = CipherValue{
      .p0_x0 = z0,
      .p0_x1 = cast(z1, 0),
      .p1_x1 = z1,
      .p1_x2 = cast(z2, 1),
      .p2_x2 = z2,
      .p2_x0 = cast(z0, 2),
  };

  return result;
}
} // namespace

auto multiply_aa(FluxBuilder &builder, CipherValue x,
                 CipherValue y) -> CipherValue {
  auto &context = builder.context();
  auto shape = context.type(x.p0_x0).shape;
  auto rng = [&](size_t x, size_t y) { return builder.random(x, y, shape); };
  auto add = [&](OpHandle x, OpHandle y) { return builder.add(x, y); };
  auto mul = [&](OpHandle x, OpHandle y) { return builder.multiply(x, y); };
  auto sub = [&](OpHandle x, OpHandle y) { return builder.subtract(x, y); };
  auto cast = [&](OpHandle x, size_t y) { return builder.cast(x, y); };
  return mul_impl(rng, add, mul, sub, cast, x, y);
}

auto matmul_aa(FluxBuilder &builder, CipherValue x,
               CipherValue y) -> CipherValue {
  auto &context = builder.context();
  size_t row = context.shape(x.p0_x0)[0];
  size_t column = context.shape(y.p0_x0)[1];
  auto shape = builder.push(Shape{row, column});
  auto rng = [&](size_t x, size_t y) { return builder.random(x, y, shape); };
  auto add = [&](OpHandle x, OpHandle y) { return builder.add(x, y); };
  auto mul = [&](OpHandle x, OpHandle y) { return builder.matmul(x, y); };
  auto sub = [&](OpHandle x, OpHandle y) { return builder.subtract(x, y); };
  auto cast = [&](OpHandle x, size_t y) { return builder.cast(x, y); };
  return mul_impl(rng, add, mul, sub, cast, x, y);
}

auto and_bb(FluxBuilder &builder, CipherValue x, CipherValue y) -> CipherValue {
  auto &context = builder.context();
  auto shape = context.type(x.p0_x0).shape;
  auto rng = [&](size_t x, size_t y) { return builder.random(x, y, shape); };
  auto add = [&](OpHandle x, OpHandle y) { return builder._xor(x, y); };
  auto mul = [&](OpHandle x, OpHandle y) { return builder._and(x, y); };
  auto sub = [&](OpHandle x, OpHandle y) { return builder._xor(x, y); };
  auto cast = [&](OpHandle x, size_t y) { return builder.cast(x, y); };
  return mul_impl(rng, add, mul, sub, cast, x, y);
}

} // namespace fastmpc::flux::aby3