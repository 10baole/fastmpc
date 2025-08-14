#pragma once

#include "fastmpc/abp/dialect/abp_builder.h"

namespace fastmpc::abp {

auto equal(ABPBuilder &builder, OpHandle left, OpHandle right) -> OpHandle;

auto less(ABPBuilder &builder, OpHandle left, OpHandle right) -> OpHandle;

auto greater(ABPBuilder &builder, OpHandle left, OpHandle right) -> OpHandle;

auto greater_equal(ABPBuilder &builder, OpHandle left, OpHandle right)
    -> OpHandle;

auto select(ABPBuilder &builder, OpHandle pred, OpHandle on_true,
            OpHandle on_false) -> OpHandle;

auto max(ABPBuilder &builder, OpHandle left, OpHandle right) -> OpHandle;
} // namespace fastmpc::abp