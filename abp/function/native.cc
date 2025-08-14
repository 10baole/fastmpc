#include "fastmpc/abp/abp_context.h"
#include "fastmpc/abp/function/function.h"

#include <cassert>
#include <cmath>
#include <cstddef>
#include <cstdio>
#include <cstdlib>

namespace fastmpc::abp::function {

namespace {

constexpr auto encode(TypeKind left, TypeKind right) -> int8_t {
  return static_cast<int8_t>(left) * 3 + static_cast<int8_t>(right);
}

} // namespace

auto subtract(ABPBuilder &builder, size_t left, size_t right) -> size_t {
  return add(builder, left, negate(builder, right));
}

namespace unsafe {
auto multiply(ABPBuilder &builder, size_t left, size_t right) -> size_t {
  auto &context = builder.context();
  auto left_kind = context.type(left).kind;
  auto right_kind = context.type(right).kind;
  switch (encode(left_kind, right_kind)) {
  case encode(TypeKind::kArithFixed64, TypeKind::kArithFixed64):
    return builder.multiply_aa(left, right);
  case encode(TypeKind::kArithFixed64, TypeKind::kFixed64):
    return builder.multiply_ap(left, right);
  case encode(TypeKind::kFixed64, TypeKind::kArithFixed64):
    return builder.multiply_ap(right, left);
  case encode(TypeKind::kFixed64, TypeKind::kFixed64):
    return builder.multiply_pp(left, right);
  default:
    std::abort();
  }
}

auto dot_general(ABPBuilder &builder, size_t left, size_t right) -> size_t {
  auto &context = builder.context();
  auto left_kind = context.type(left).kind;
  auto right_kind = context.type(right).kind;
  switch (encode(left_kind, right_kind)) {
  case encode(TypeKind::kArithFixed64, TypeKind::kArithFixed64):
    return builder.dot_general_aa(left, right);
  default:
    std::abort();
  }
}

} // namespace unsafe

auto multiply(ABPBuilder &builder, size_t left, size_t right) -> size_t {
  auto &context = builder.context();
  auto result = unsafe::multiply(builder, left, right);
  uint8_t fixed_point = context.type(result).fixed_point;
  if (fixed_point > builder.fixed_point()) {
    uint8_t bits = fixed_point - builder.fixed_point();
    result = truncate(builder, result, bits);
  }
  return result;
}

auto dot_general(ABPBuilder &builder, size_t left, size_t right) -> size_t {
  auto &context = builder.context();
  auto result = unsafe::dot_general(builder, left, right);
  uint8_t fixed_point = context.type(result).fixed_point;
  if (fixed_point > builder.fixed_point()) {
    uint8_t bits = fixed_point - builder.fixed_point();
    result = truncate(builder, result, bits);
  }
  return result;
}

auto bitwise_and(ABPBuilder &builder, size_t left, size_t right) -> size_t {
  auto &context = builder.context();
  auto left_kind = context.type(left).kind;
  auto right_kind = context.type(right).kind;
  switch (encode(left_kind, right_kind)) {
  case encode(TypeKind::kBitArray64, TypeKind::kBitArray64):
    return builder.and_bb(left, right);
  default:
    std::abort();
  }
}

auto bitwise_xor(ABPBuilder &builder, size_t left, size_t right) -> size_t {
  auto &context = builder.context();
  auto left_kind = context.type(left).kind;
  auto right_kind = context.type(right).kind;
  switch (encode(left_kind, right_kind)) {
  case encode(TypeKind::kBitArray64, TypeKind::kBitArray64):
    return builder.xor_bb(left, right);
  default:
    std::abort();
  }
}

auto bitwise_or(ABPBuilder &builder, size_t left, size_t right) -> size_t {
  auto &context = builder.context();
  auto left_kind = context.type(left).kind;
  auto right_kind = context.type(right).kind;
  switch (encode(left_kind, right_kind)) {
  case encode(TypeKind::kBitArray64, TypeKind::kBitArray64):
    return builder.xor_bb(left,
                          builder.xor_bb(right, builder.and_bb(left, right)));
  default:
    std::abort();
  }
}

auto negate(ABPBuilder &builder, size_t operand) -> size_t {
  auto &context = builder.context();
  auto operand_kind = context.type(operand).kind;
  switch (operand_kind) {
  case TypeKind::kArithFixed64:
    return builder.negate_a(operand);
  case TypeKind::kFixed64:
    return builder.negate_p(operand);
  default:
    std::abort();
  }
}

auto truncate(ABPBuilder &builder, size_t operand, uint8_t bits) -> size_t {
  auto &context = builder.context();
  auto operand_kind = context.type(operand).kind;
  switch (operand_kind) {
  case TypeKind::kArithFixed64:
    return builder.truncate_a(operand, bits);
  case TypeKind::kFixed64:
    return builder.truncate_p(operand, bits);
  default:
    std::abort();
  }
}

auto a2b(ABPBuilder &builder, size_t operand) -> size_t {
  auto &context = builder.context();
  auto operand_kind = context.type(operand).kind;
  switch (operand_kind) {
  case TypeKind::kArithFixed64:
    return builder.a2b(operand);
  default:
    std::abort();
  }
}

auto b2a(ABPBuilder &builder, size_t operand, uint8_t fixed_point) -> size_t {
  auto &context = builder.context();
  auto operand_kind = context.type(operand).kind;
  switch (operand_kind) {
  case TypeKind::kBitArray64:
    return builder.b2a(operand, fixed_point);
  default:
    std::abort();
  }
}

auto constant(ABPBuilder &builder, float value) -> size_t {
  return constant(builder, std::vector<float>{value}, {});
}

auto constant(ABPBuilder &builder, uint64_t value) -> size_t {
  return constant(builder, std::vector<uint64_t>{value}, {});
}

namespace {

auto encode(std::vector<float> &&in, uint8_t fixed_point) {
  std::vector<uint64_t> out(in.size());
  for (size_t i = 0; i < in.size(); i++) {
    out[i] = static_cast<uint64_t>(
        static_cast<int64_t>(in[i] * (1ll << fixed_point)));
  }
  return out;
}

} // namespace

auto constant(ABPBuilder &builder, std::vector<float> &&value,
              std::vector<size_t> &&shape) -> size_t {
  auto output_type = builder.make_type(TypeKind::kFixed64,
                                       builder.fixed_point(), std::move(shape));
  return builder.constant(encode(std::move(value), builder.fixed_point()),
                          output_type);
}

auto constant(ABPBuilder &builder, std::vector<uint64_t> &&value,
              std::vector<size_t> &&shape) -> size_t {
  auto output_type = builder.make_type(TypeKind::kFixed64, 0, std::move(shape));
  return builder.constant(std::move(value), output_type);
}

namespace unsafe {
auto constant(ABPBuilder &builder, uint64_t value, uint8_t fixed_point,
              size_t handle) -> size_t {
  auto context = builder.context();

  auto output_type =
      builder.make_type(TypeKind::kFixed64, fixed_point, std::vector<size_t>{});

  size_t result = builder.constant({value}, output_type);
  if (context.shape(handle).empty()) {
    return result;
  }
  return builder.broadcast(result, {}, handle);
}

auto constant(ABPBuilder &builder, float value, size_t handle) {
  uint8_t fixed_point = builder.fixed_point();
  auto signed_value = static_cast<int64_t>(value * (1ll << fixed_point));
  return constant(builder, static_cast<uint64_t>(signed_value), fixed_point,
                  handle);
}

auto iota(ABPBuilder &builder, size_t iota_dim, uint8_t fixed_point,
          std::vector<size_t> &&shape) -> size_t {
  size_t n = shape[iota_dim];
  std::vector<uint64_t> data(n);
  for (size_t i = 0; i < n; i++) {
    data[i] = i << fixed_point;
  }
  if (shape.size() == 1) {
    auto output_type =
        builder.make_type(TypeKind::kFixed64, fixed_point, std::move(shape));
    return builder.constant(std::move(data), output_type);
  } else {
    auto output_type = builder.make_type(TypeKind::kFixed64, fixed_point,
                                         std::vector<size_t>{n});
    auto iota_result = builder.constant(std::move(data), output_type);
    return builder.broadcast(iota_result, {iota_dim}, std::move(shape));
  }
}

} // namespace unsafe

auto select(ABPBuilder &builder, size_t which, size_t left, size_t right)
    -> size_t {
  auto diff = subtract(builder, left, right);
  auto false_path = multiply(builder, diff, which);
  return add(builder, right, false_path);
}

auto max(ABPBuilder &builder, size_t left, size_t right) -> size_t {
  auto which = greater(builder, left, right);
  return select(builder, which, left, right);
}

namespace {

auto msb(ABPBuilder &builder, size_t x) {
  auto a_type_value = builder.a2b(x);
  auto b_result = builder.shift_right(a_type_value, 63);
  auto a_result = builder.b2a(b_result, 0);
  return a_result;
}

auto perfix_or(ABPBuilder &builder, size_t x) {
  for (size_t i = 0; i < 6; i++) {
    auto doubled = builder.shift_right(x, 1 << i);
    x = function::bitwise_or(builder, x, doubled);
  }
  return x;
}

auto highest_one_bit(ABPBuilder &builder, size_t x) {
  x = builder.a2b(x);
  x = perfix_or(builder, x);
  auto y = builder.shift_right(x, 1);
  return builder.xor_bb(x, y);
}

auto bit_reverse(ABPBuilder &builder, size_t x, uint8_t perfix) {
  x = builder.bit_reverse(x);
  x = builder.shift_right(x, 64 - perfix);
  return builder.b2a(x, builder.fixed_point());
}

auto divide_xa(ABPBuilder &builder, size_t x, size_t y) -> size_t {
  auto is_negative = msb(builder, y);
  auto y_abs = select(builder, is_negative, negate(builder, y), y);
  auto y_msb = highest_one_bit(builder, y_abs);
  auto factor = bit_reverse(builder, y_msb, 2 * builder.fixed_point());
  x = multiply(builder, x, factor);
  y = multiply(builder, y_abs, factor);
  for (size_t i = 0; i < 5; i++) {
    x = subtract(builder, add(builder, x, x), multiply(builder, x, y));
    if (i + 1 != 5)
      y = subtract(builder, add(builder, y, y), multiply(builder, y, y));
  }
  return select(builder, is_negative, negate(builder, x), x);
}

auto divide_xp(ABPBuilder &builder, size_t x, size_t y) -> size_t {
  assert(builder.check_shape(x, y));
  return multiply(builder, x, builder.inverse(y));
}

auto log2_int(ABPBuilder &builder, size_t x) {
  auto &context = builder.context();
  uint8_t fixed_point = builder.fixed_point();
  x = truncate(builder, x, fixed_point);
  auto x_shape = context.type(x).shape;

  auto make_value = [&](uint64_t x) {
    return unsafe::constant(builder, x, 0, x_shape);
  };

  size_t result;
  for (size_t i = 0; i < 32 - fixed_point; i++) {
    size_t n = 31 - fixed_point - i;
    auto y = builder.divide_pow_of_2(x, n);
    size_t mask = multiply(builder, y, make_value(n));
    if (i == 0) {
      result = mask;
    } else {
      result = add(builder, result, mask);
    }
    if (n != 0) {
      y = multiply(builder, y, make_value(1ull << n));
      x = subtract(builder, x, y);
    }
  }
  size_t ones =
      unsafe::constant(builder, 1 << fixed_point, fixed_point, x_shape);
  return multiply(builder, result, ones);
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
auto log2_pade_normalized(ABPBuilder &builder, size_t x) {
  auto &context = builder.context();
  auto x_shape = context.type(x).shape;
  auto x2 = multiply(builder, x, x);
  auto x3 = multiply(builder, x, x2);

  auto p0 = unsafe::constant(builder, -0.205466671951F * 10, x_shape);
  auto p1 = unsafe::constant(builder, -0.88626599391F * 10, x_shape);
  auto p2 = unsafe::constant(builder, 0.610585199015F * 10, x_shape);
  auto p3 = unsafe::constant(builder, 0.481147460989F * 10, x_shape);

  auto q0 = unsafe::constant(builder, 0.353553425277F, x_shape);
  auto q1 = unsafe::constant(builder, 0.454517087629F * 10, x_shape);
  auto q2 = unsafe::constant(builder, 0.642784209029F * 10, x_shape);
  auto q3 = unsafe::constant(builder, 0.1F * 10, x_shape);

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

auto log2(ABPBuilder &builder, size_t x) -> size_t {
  uint8_t fixed_point = builder.fixed_point();
  auto msb_b = highest_one_bit(builder, x);
  auto msb_a = builder.b2a(msb_b, fixed_point);
  auto factor = bit_reverse(builder, msb_b, 2 * fixed_point);
  x = multiply(builder, x, factor);
  // return log2_pade_normalized(builder, x);
  return add(builder, log2_pade_normalized(builder, x),
             log2_int(builder, add(builder, msb_a, msb_a)));
}

auto divide(ABPBuilder &builder, size_t x, size_t y) -> size_t {
  auto &context = builder.context();
  assert(context.type(x).kind != TypeKind::kBitArray64);
  switch (context.type(y).kind) {
  case TypeKind::kArithFixed64:
    return divide_xa(builder, x, y);
  case TypeKind::kFixed64:
    return divide_xp(builder, x, y);
  case TypeKind::kBitArray64:
    std::abort();
  }
}

auto exp(ABPBuilder &builder, size_t x) -> size_t {
  auto &context = builder.context();
  const size_t n = 12;
  auto left = builder.divide_pow_of_2(x, n);
  auto right = unsafe::constant(builder, 1 << builder.fixed_point(),
                                builder.fixed_point(), context.type(x).shape);
  auto result = add(builder, left, right);
  for (size_t i = 0; i < n; i++) {
    result = multiply(builder, result, result);
  }
  return result;
}

auto log(ABPBuilder &builder, size_t x) -> size_t {
  auto &context = builder.context();
  auto x_shape = context.type(x).shape;
  auto left = unsafe::constant(builder, std::log(2.0f), x_shape);
  auto right = log2(builder, x);
  return multiply(builder, left, right);
}

} // namespace fastmpc::abp::function