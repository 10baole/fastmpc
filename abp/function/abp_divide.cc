#include "fastmpc/abp/function/abp_divide.h"

#include <cassert>
#include <cstdlib>
#include <type_traits>

#include "fastmpc/abp/dialect/abp_ops.h"
#include "fastmpc/abp/function/abp_binary.h"
#include "fastmpc/abp/function/abp_circuit.h"
#include "fastmpc/abp/function/abp_compare.h"
#include "fastmpc/abp/function/abp_unary.h"

namespace fastmpc::abp {

namespace {

auto divide_xa(ABPBuilder &builder, OpHandle x, OpHandle y) -> OpHandle {
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

auto divide_xp(ABPBuilder &builder, OpHandle x, OpHandle y) -> OpHandle {
  return multiply(builder, x, builder.inverse(y));
}
} // namespace

auto divide(ABPBuilder &builder, OpHandle x, OpHandle y) -> OpHandle {
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

} // namespace fastmpc::abp