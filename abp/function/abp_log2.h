#pragma once

#include "fastmpc/abp/dialect/abp_builder.h"

namespace fastmpc::abp {
auto log2(ABPBuilder &builder, OpHandle x) -> OpHandle;
}