#include "fastmpc/flux/low/aby3/function/aby3_casting.h"

#include "fastmpc/flux/dialect/flux_builder.h"
#include "fastmpc/flux/low/3pc/function/3pc_unary.h"
#include "fastmpc/flux/low/aby3/aby3_value.h"
#include "fastmpc/flux/low/aby3/function/aby3_binary.h"

namespace fastmpc::flux::aby3 {

namespace {

auto kogge_stone(FluxBuilder &builder, CipherValue x,
                 CipherValue y) -> CipherValue {
  auto P = xor_bb(builder, x, y);
  auto G = and_bb(builder, x, y);

  constexpr size_t rounds = 63 - __builtin_clzll(64);
  for (size_t idx = 0; idx < rounds; idx++) {
    auto G1 = _3pc::shift_left(builder, cast(G), 1 << idx);
    auto P1 = _3pc::shift_left(builder, cast(P), 1 << idx);
    auto tmp = and_bb(builder, P, cast(G1));
    P = and_bb(builder, P, cast(P1));
    G = xor_bb(builder, G, tmp);
  }
  auto C = _3pc::shift_left(builder, cast(G), 1);
  return xor_bb(builder, x, xor_bb(builder, y, cast(C)));
}

} // namespace

auto a2b(FluxBuilder &builder, CipherValue in) -> CipherValue {
  auto &context = builder.context();
  auto shape = context.type(in.p0_x0).shape;
  auto make_zeros = [&](size_t holder) {
    Type type{
        .holder = holder,
        .shape = shape,
    };
    auto result = builder.constant({0}, type);
    if (!context.shape(shape).empty()) {
      result = builder.broadcast(result, {}, shape);
    }
    return result;
  };
  auto x = CipherValue{
      .p0_x0 = make_zeros(0),
      .p0_x1 = in.p0_x1,
      .p1_x1 = in.p1_x1,
      .p1_x2 = make_zeros(1),
      .p2_x2 = make_zeros(2),
      .p2_x0 = make_zeros(2),
  };
  auto [p0_r0, p1_r0] = builder.random(0, 1, shape);
  auto [p1_r1, p2_r1] = builder.random(1, 2, shape);
  auto [p2_r2, p0_r2] = builder.random(2, 0, shape);
  auto z0 = builder._xor(p0_r2, p0_r0);
  auto z1 = builder._xor(p1_r0, p1_r1);
  auto z2 = builder._xor(p2_r1, p2_r2);
  auto x0_x2 = builder.add(in.p2_x0, in.p2_x2);
  auto x0_x2_z2 = builder._xor(x0_x2, z2);

  auto y = CipherValue{
      .p0_x0 = z0,
      .p0_x1 = builder.cast(z1, 0),
      .p1_x1 = z1,
      .p1_x2 = builder.cast(x0_x2_z2, 1),
      .p2_x2 = x0_x2_z2,
      .p2_x0 = builder.cast(z0, 2),
  };
  return kogge_stone(builder, x, y);
}

auto b2a(FluxBuilder &builder, CipherValue in) -> CipherValue {
  auto &context = builder.context();
  auto shape = context.type(in.p0_x0).shape;
  auto [p1_x2, p2_x2] = builder.random(1, 2, shape);
  auto [p2_x0, p0_x0] = builder.random(2, 0, shape);

  auto [p0_r0, p1_r0] = builder.random(0, 1, shape);
  auto [p1_r1, p2_r1] = builder.random(1, 2, shape);
  auto [p2_r2, p0_r2] = builder.random(2, 0, shape);
  auto z0 = builder._xor(p0_r2, p0_r0);
  auto z1 = builder._xor(p1_r0, p1_r1);
  auto z2 = builder._xor(p2_r1, p2_r2);
  auto x0_x2 = builder.negate(builder.add(p2_x2, p2_x0));
  auto x0_x2_z2 = builder._xor(x0_x2, z2);
  auto y = CipherValue{
      .p0_x0 = z0,
      .p0_x1 = builder.cast(z1, 0),
      .p1_x1 = z1,
      .p1_x2 = builder.cast(x0_x2_z2, 1),
      .p2_x2 = x0_x2_z2,
      .p2_x0 = builder.cast(z0, 2),
  };

  auto x1 = kogge_stone(builder, in, y);

  auto p0_x1 = builder._xor(x1.p0_x0, x1.p0_x1);
  p0_x1 = builder._xor(p0_x1, builder.cast(x1.p1_x2, 0));

  auto p1_x1 = builder._xor(x1.p1_x1, x1.p1_x2);
  p1_x1 = builder._xor(p1_x1, builder.cast(x1.p2_x0, 1));

  return CipherValue{
      .p0_x0 = p0_x0,
      .p0_x1 = p0_x1,
      .p1_x1 = p1_x1,
      .p1_x2 = p1_x2,
      .p2_x2 = p2_x2,
      .p2_x0 = p2_x0,
  };
}

} // namespace fastmpc::flux::aby3