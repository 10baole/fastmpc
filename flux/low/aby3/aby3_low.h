#pragma once

#include "fastmpc/abp/dialect/abp_context.h"
#include "fastmpc/flux/dialect/flux_context.h"

namespace fastmpc::flux::aby3 {
auto low(const abp::ABPContext &context) -> FluxContext;
}