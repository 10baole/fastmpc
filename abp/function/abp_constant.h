#pragma once
#include "fastmpc/abp/dialect/abp_builder.h"

using namespace std;

namespace fastmpc::abp {

    auto constant(ABPBuilder &builder, float value)    -> OpHandle;
    auto constant(ABPBuilder &builder, uint64_t value) -> OpHandle;
    auto constant(ABPBuilder &builder, vector <float> &&value, Shape &&shape)    -> OpHandle;
    auto constant(ABPBuilder &builder, vector <uint64_t> &&value, Shape &&shape) -> OpHandle;
    auto iota(ABPBuilder &builder, size_t iota_dimension, Shape &&shape)         -> OpHandle;
    
}