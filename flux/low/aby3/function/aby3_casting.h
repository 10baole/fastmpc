#pragma once

#include "fastmpc/flux/dialect/flux_builder.h"
#include "fastmpc/flux/low/aby3/aby3_value.h"

namespace fastmpc::flux::aby3 {

auto a2b(FluxBuilder &builder, CipherValue x) -> CipherValue;

auto b2a(FluxBuilder &builder, CipherValue x) -> CipherValue;

} // namespace fastmpc::flux::aby3