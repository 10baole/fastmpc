#pragma once

#include "fastmpc/abp/dialect/abp_builder.h"

namespace fastmpc::abp {

auto divide(ABPBuilder &builder, OpHandle x, OpHandle y) -> OpHandle;

}