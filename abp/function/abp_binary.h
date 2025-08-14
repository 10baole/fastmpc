#pragma once
#include "fastmpc/abp/dialect/abp_builder.h"

namespace fastmpc::abp {

    auto add(ABPBuilder &builder, OpHandle left, OpHandle right) -> OpHandle;
    auto subtract(ABPBuilder &builder, OpHandle left, OpHandle right) -> OpHandle;
    auto multiply(ABPBuilder &builder, OpHandle left, OpHandle right) -> OpHandle;
    auto dot_general(ABPBuilder &builder, OpHandle left, OpHandle right) -> OpHandle;
    
    auto bitwise_xor(ABPBuilder &builder, OpHandle left, OpHandle right) -> OpHandle;
    auto bitwise_or(ABPBuilder &builder, OpHandle left, OpHandle right) -> OpHandle;

}