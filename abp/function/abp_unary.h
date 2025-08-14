#pragma once
#include "fastmpc/abp/dialect/abp_builder.h"

namespace fastmpc::abp {

    auto negate(ABPBuilder &builder, OpHandle operand) -> OpHandle;
    auto truncate(ABPBuilder &builder, OpHandle operand, uint8_t bits) -> OpHandle;

    auto a2b(ABPBuilder &builder, OpHandle operand) -> OpHandle;
    auto b2a(ABPBuilder &builder, OpHandle operand, uint8_t fixed_point) -> OpHandle;

    auto exp(ABPBuilder &builder, OpHandle x) -> OpHandle;
    auto log(ABPBuilder &builder, OpHandle x) -> OpHandle;
    auto abs(ABPBuilder &builder, OpHandle x) -> OpHandle;
    auto sqrt(ABPBuilder &builder, OpHandle x) -> OpHandle;
    auto erf(ABPBuilder &builder, OpHandle x) -> OpHandle;

}