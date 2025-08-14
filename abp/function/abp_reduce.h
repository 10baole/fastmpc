#pragma once
#include "fastmpc/abp/dialect/abp_builder.h"

using namespace std;

namespace fastmpc::abp {

    using ReduceFunc = auto (*)(ABPBuilder &, OpHandle, OpHandle) -> OpHandle;
    auto reduce(ABPBuilder &builder, OpHandle x, OpHandle init, DenseSizeT &&dims, Shape &&shape, ReduceFunc reducer) -> OpHandle;
    auto reduce_sum(ABPBuilder &builder, OpHandle operand, const vector <size_t> &axis) -> OpHandle;
    auto reduce_max(ABPBuilder &builder, OpHandle operand, const vector <size_t> &axis) -> OpHandle;

}