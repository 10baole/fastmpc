#include "fastmpc/abp/function/function.h"

namespace fastmpc::abp::function {

namespace {

auto msb(ABPBuilder &builder, size_t operand) {
  return builder.shift_right(builder.a2b(operand), 63);
}

// msb(left - right)
auto less_b(ABPBuilder &builder, size_t left, size_t right) -> size_t {
  return msb(builder, subtract(builder, left, right));
}

// less(right, left)
auto greater_b(ABPBuilder &builder, size_t left, size_t right) -> size_t {
  return less_b(builder, right, left);
}

} // namespace

// !less(left, right) && !greater(left, right)
auto equal(ABPBuilder &builder, size_t left, size_t right) -> size_t {
  auto not_less = builder.not_b(less_b(builder, left, right));
  auto not_greater = builder.not_b(greater_b(builder, left, right));
  auto result_b = builder.and_bb(not_less, not_greater);
  return builder.b2a(result_b, 0);
}

auto less(ABPBuilder &builder, size_t left, size_t right) -> size_t {
  return builder.b2a(less_b(builder, left, right), 0);
}

auto greater(ABPBuilder &builder, size_t left, size_t right) -> size_t {
  return builder.b2a(greater_b(builder, left, right), 0);
}

// !less(left, right)
auto greater_equal(ABPBuilder &builder, size_t left, size_t right) -> size_t {
  auto not_less = builder.not_b(less_b(builder, left, right));
  return builder.b2a(not_less, 0);
}

} // namespace fastmpc::abp::function