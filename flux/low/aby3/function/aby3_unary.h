#pragma once

#include "fastmpc/flux/dialect/flux_builder.h"
#include "fastmpc/flux/low/aby3/aby3_value.h"

namespace fastmpc::flux::aby3 {

auto truncate_a(FluxBuilder &builder, CipherValue x,
                uint8_t bits) -> CipherValue;

} // namespace fastmpc::flux::aby3