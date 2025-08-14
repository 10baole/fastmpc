#include "fastmpc/abp/function/abp_unary.h"

#include <cmath>
#include <cstdlib>

#include "fastmpc/abp/dialect/abp_ops.h"
#include "fastmpc/abp/function/abp_binary.h"
#include "fastmpc/abp/function/abp_circuit.h"
#include "fastmpc/abp/function/abp_compare.h"
#include "fastmpc/abp/function/abp_constant.h"
#include "fastmpc/abp/function/abp_log2.h"

namespace fastmpc::abp {

auto negate(ABPBuilder &builder, OpHandle operand) -> OpHandle {
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

auto truncate(ABPBuilder &builder, OpHandle operand, uint8_t bits) -> OpHandle {
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

auto a2b(ABPBuilder &builder, OpHandle operand) -> OpHandle {
  auto &context = builder.context();
  auto operand_kind = context.type(operand).kind;
  switch (operand_kind) {
  case TypeKind::kArithFixed64:
    return builder.a2b(operand);
  default:
    std::abort();
  }
}

auto b2a(ABPBuilder &builder, OpHandle operand,
         uint8_t fixed_point) -> OpHandle {
  auto &context = builder.context();
  auto operand_kind = context.type(operand).kind;
  switch (operand_kind) {
  case TypeKind::kBitArray64:
    return builder.b2a(operand, fixed_point);
  default:
    std::abort();
  }
}

auto exp(ABPBuilder &builder, OpHandle x) -> OpHandle {
  auto &context = builder.context();
  const size_t n = 12;
  auto left = builder.divide_pow_of_2(x, n);
  auto right = constant(builder, 1.0f);

  if (!context.shape(left).empty()) {
    auto shape = context.type(x).shape;
    right = builder.broadcast(right, {}, shape);
  }
  auto result = add(builder, left, right);
  for (size_t i = 0; i < n; i++) {
    result = multiply(builder, result, result);
  }
  return result;
}

auto log(ABPBuilder &builder, OpHandle x) -> OpHandle {
  auto &context = builder.context();
  auto x_shape = context.type(x).shape;
  auto left = constant(builder, std::log(2.0f));
  if (!context.shape(x_shape).empty()) {
    left = builder.broadcast(left, {}, x_shape);
  }
  auto right = log2(builder, x);
  return multiply(builder, left, right);
}

auto abs(ABPBuilder &builder, OpHandle x) -> OpHandle {
  auto is_negative = msb(builder, x);
  auto x_neg = negate(builder, x);
  return select(builder, is_negative, x_neg, x);
}

} // namespace fastmpc::abp