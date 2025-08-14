#pragma once

#include "fastmpc/abp/dialect/abp_builder.h"

namespace fastmpc::abp {

auto constant(ABPBuilder &builder, float value) -> OpHandle;

auto constant(ABPBuilder &builder, uint64_t value) -> OpHandle;

auto constant(ABPBuilder &builder, std::vector<float> &&value, Shape &&shape)
    -> OpHandle;

auto constant(ABPBuilder &builder, std::vector<uint64_t> &&value, Shape &&shape)
    -> OpHandle;

auto iota(ABPBuilder &builder, size_t iota_dimension, Shape &&shape)
    -> OpHandle;
} // namespace fastmpc::abp