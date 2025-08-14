#include "fastmpc/abp/function/abp_circuit.h"

#include "fastmpc/abp/function/abp_binary.h"

namespace fastmpc::abp {

auto msb(ABPBuilder &builder, OpHandle x) -> OpHandle {
  auto a_type_value = builder.a2b(x);
  auto b_result = builder.shift_right(a_type_value, 63);
  auto a_result = builder.b2a(b_result, 0);
  return a_result;
}

auto perfix_or(ABPBuilder &builder, OpHandle x) -> OpHandle {
  for (size_t i = 0; i < 6; i++) {
    auto doubled = builder.shift_right(x, 1 << i);
    x = bitwise_or(builder, x, doubled);
  }
  return x;
}

auto highest_one_bit(ABPBuilder &builder, OpHandle x) -> OpHandle {
  x = builder.a2b(x);
  x = perfix_or(builder, x);
  auto y = builder.shift_right(x, 1);
  return builder.xor_bb(x, y);
}

auto bit_reverse(ABPBuilder &builder, OpHandle x, uint8_t perfix) -> OpHandle {
  x = builder.bit_reverse(x);
  x = builder.shift_right(x, 64 - perfix);
  return builder.b2a(x, builder.fixed_point());
}

} // namespace fastmpc::abp