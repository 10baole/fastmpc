#include "fastmpc/abp/function/abp_log2.h"

#include "fastmpc/abp/function/abp_binary.h"
#include "fastmpc/abp/function/abp_circuit.h"
#include "fastmpc/abp/function/abp_constant.h"
#include "fastmpc/abp/function/abp_function.h"
#include "fastmpc/abp/function/abp_unary.h"

namespace fastmpc::abp {

namespace {

auto log2_int(ABPBuilder &builder, OpHandle x) {
  auto &context = builder.context();
  uint8_t fixed_point = builder.fixed_point();
  auto x_shape = context.type(x).shape;
  auto make_value = [&](auto x) {
    auto result = constant(builder, x);
    if (!context.shape(x_shape).empty()) {
      result = builder.broadcast(result, {}, x_shape);
    }
    return result;
  };
  x = a2b(builder, x);
  x = perfix_or(builder, x);

  auto k1 = make_value((uint64_t)1);
  k1 = builder.p2a(k1);
  k1 = builder.a2b(k1);

  OpHandle result = make_value(-(uint64_t)fixed_point);
  for (size_t i = 0; i < 32; i++) {
    auto bit = builder.shift_right(x, i);
    bit = builder.and_bb(bit, k1);
    bit = builder.b2a(bit, 0);
    result = add(builder, result, bit);
  }
  return multiply(builder, result, make_value(1.f));
}

// Pade approximation fo x belongs to [0.5, 1]:
//
// p2524(x) = -0.205466671951 * 10
//          + x * -0.88626599391 * 10
//          + x^2 * 0.610585199015 * 10
//          + x^3 * 0.481147460989 * 10
// q2524(x) = 0.353553425277
//          + x * 0.454517087629 * 10
//          + x^2 * 0.642784209029 * 10
//          + x^3 * 0.1 *10
// log2(x) = p2524(x) / q2524(x)
auto log2_pade_normalized(ABPBuilder &builder, OpHandle x) {
  auto &context = builder.context();
  auto x_shape = context.type(x).shape;
  auto x2 = multiply(builder, x, x);
  auto x3 = multiply(builder, x, x2);

  auto make_value = [&](auto x) {
    auto result = constant(builder, x);
    if (!context.shape(x_shape).empty()) {
      result = builder.broadcast(result, {}, x_shape);
    }
    return result;
  };

  auto p0 = make_value(-0.205466671951F * 10);
  auto p1 = make_value(-0.88626599391F * 10);
  auto p2 = make_value(0.610585199015F * 10);
  auto p3 = make_value(0.481147460989F * 10);

  auto q0 = make_value(0.353553425277F);
  auto q1 = make_value(0.454517087629F * 10);
  auto q2 = make_value(0.642784209029F * 10);
  auto q3 = make_value(0.1F * 10);

  auto p2524 = multiply(builder, x, p1);
  p2524 = add(builder, p2524, multiply(builder, x2, p2));
  p2524 = add(builder, p2524, multiply(builder, x3, p3));
  p2524 = add(builder, p2524, p0);

  auto q2524 = multiply(builder, x, q1);
  q2524 = add(builder, q2524, multiply(builder, x2, q2));
  q2524 = add(builder, q2524, multiply(builder, x3, q3));
  q2524 = add(builder, q2524, q0);
  return divide(builder, p2524, q2524);
}

} // namespace

auto log2(ABPBuilder &builder, OpHandle x) -> OpHandle {
  // return log2_int(builder, x);
  uint8_t fixed_point = builder.fixed_point();
  auto msb_b = highest_one_bit(builder, x);
  auto msb_a = builder.b2a(msb_b, fixed_point);
  auto factor = bit_reverse(builder, msb_b, 2 * fixed_point);
  x = multiply(builder, x, factor);
  return add(builder, log2_pade_normalized(builder, x), log2_int(builder, msb_a));
}

} // namespace fastmpc::abp