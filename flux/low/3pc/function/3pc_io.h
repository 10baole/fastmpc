#pragma once

#include "fastmpc/flux/dialect/flux_builder.h"
#include "fastmpc/flux/low/3pc/3pc_value.h"

namespace fastmpc::flux::_3pc {

template <class Ret>
auto input(FluxBuilder &builder, size_t input_index, ShapeHandle shape) -> Ret;

void output(FluxBuilder &builder, size_t output_index, PlainValue value);

void output(FluxBuilder &builder, size_t output_index, CipherValue value);

} // namespace fastmpc::flux::_3pc