#pragma once
#include "fastmpc/flux/dialect/flux_context.h"
#include "fastmpc/abp/dialect/abp_context.h"

namespace fastmpc::flux::_3pc {

    auto low(const abp::ABPContext &context) -> FluxContext;
    
}