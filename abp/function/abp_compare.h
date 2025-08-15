#pragma once
#include "fastmpc/abp/dialect/abp_builder.h"

namespace fastmpc::abp {

    auto   isEqual(ABPBuilder &builder, OpHandle left, OpHandle right) -> OpHandle;
    auto    isLess(ABPBuilder &builder, OpHandle left, OpHandle right) -> OpHandle;
    auto isGreater(ABPBuilder &builder, OpHandle left, OpHandle right) -> OpHandle;
    auto     isGEQ(ABPBuilder &builder, OpHandle left, OpHandle right) -> OpHandle;
    auto  maximize(ABPBuilder &builder, OpHandle left, OpHandle right) -> OpHandle;
    auto selectOne(ABPBuilder &builder, OpHandle pred, OpHandle on_true, OpHandle on_false) -> OpHandle;

}