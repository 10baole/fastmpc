#include "fastmpc/abp/function/abp_compare.h"

#include "fastmpc/abp/function/abp_binary.h"

namespace fastmpc::abp {

namespace {

auto msb(ABPBuilder &builder, OpHandle operand) {
  return builder.shift_right(builder.a2b(operand), 63);
}

// msb(left - right)
auto less_b(ABPBuilder &builder, OpHandle left, OpHandle right) -> OpHandle {
  return msb(builder, subtract(builder, left, right));
}

// less(right, left)
auto greater_b(ABPBuilder &builder, OpHandle left, OpHandle right) -> OpHandle {
  return less_b(builder, right, left);
}

} // namespace

// !less(left, right) && !greater(left, right)
auto equal(ABPBuilder &builder, OpHandle left, OpHandle right) -> OpHandle {
  auto not_less = builder.not_b(less_b(builder, left, right));
  auto not_greater = builder.not_b(greater_b(builder, left, right));
  auto result_b = builder.and_bb(not_less, not_greater);
  return builder.b2a(result_b, 0);
}

auto less(ABPBuilder &builder, OpHandle left, OpHandle right) -> OpHandle {
  return builder.b2a(less_b(builder, left, right), 0);
}

auto greater(ABPBuilder &builder, OpHandle left, OpHandle right) -> OpHandle {
  return builder.b2a(greater_b(builder, left, right), 0);
}

// !less(left, right)
auto greater_equal(ABPBuilder &builder, OpHandle left, OpHandle right)
    -> OpHandle {
  auto not_less = builder.not_b(less_b(builder, left, right));
  return builder.b2a(not_less, 0);
}

auto select(ABPBuilder &builder, OpHandle which, OpHandle left, OpHandle right)
    -> OpHandle {
  auto diff = subtract(builder, left, right);
  auto false_path = multiply(builder, diff, which);
  return add(builder, right, false_path);
}

auto max(ABPBuilder &builder, OpHandle left, OpHandle right) -> OpHandle {
  auto which = greater(builder, left, right);
  return select(builder, which, left, right);
}

} // namespace fastmpc::abp