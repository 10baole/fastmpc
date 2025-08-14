#pragma once
#include "fastmpc/abp/dialect/abp_builder.h"

namespace fastmpc::abp {
    
    auto softmax(ABPBuilder &builder, OpHandle x) -> OpHandle;
    auto    gelu(ABPBuilder &builder, OpHandle x) -> OpHandle;
    
}