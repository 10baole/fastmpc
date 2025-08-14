#pragma once

#include "fastmpc/abp/dialect/abp_context.h"
#include "fastmpc/pphlo/pphlo_context.h"

namespace fastmpc::abp {
auto low(const pphlo::PPHLOContext &context) -> ABPContext;
}