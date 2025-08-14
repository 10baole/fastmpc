#pragma once

#include "fastmpc/abp/dialect/abp_builder.h"

namespace fastmpc::abp {

auto msb(ABPBuilder &builder, OpHandle x) -> OpHandle;

auto perfix_or(ABPBuilder &builder, OpHandle x) -> OpHandle;

auto highest_one_bit(ABPBuilder &builder, OpHandle x) -> OpHandle;

auto bit_reverse(ABPBuilder &builder, OpHandle x, uint8_t perfix) -> OpHandle;

} // namespace fastmpc::abp