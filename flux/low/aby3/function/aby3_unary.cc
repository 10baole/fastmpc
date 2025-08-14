#include "fastmpc/flux/low/aby3/function/aby3_unary.h"

namespace fastmpc::flux::aby3 {

auto truncate_a(FluxBuilder &builder, CipherValue x,
                uint8_t bits) -> CipherValue {
  auto &context = builder.context();
  auto shape = context.type(x.p0_x0).shape;
  auto [p0_x0, p0_x1, p1_x1, p1_x2, p2_x2, p2_x0] = x;
  auto [p1_y2, p2_y2] = builder.random(1, 2, shape);
  auto p0_y0 = builder.arith_shift_right(p0_x0, bits);
  auto p2_y0 = builder.arith_shift_right(p2_x0, bits);
  auto p1_y1 = builder.add(p1_x1, p1_x2);
  p1_y1 = builder.arith_shift_right(p1_y1, bits);
  auto p0_y1 = builder.cast(p1_y1, 0);
  return CipherValue{
      .p0_x0 = p0_y0,
      .p0_x1 = p0_y1,
      .p1_x1 = p1_y1,
      .p1_x2 = p1_y2,
      .p2_x2 = p2_y2,
      .p2_x0 = p2_y0,
  };
}

} // namespace fastmpc::flux::aby3