#pragma once

#include "fastmpc/flux/dialect/flux_builder.h"
#include "fastmpc/flux/low/aby3/aby3_value.h"

namespace fastmpc::flux::aby3 {

auto add_aa(FluxBuilder &builder, CipherValue x, CipherValue y) -> CipherValue;

auto multiply_aa(FluxBuilder &builder, CipherValue x, CipherValue y)
    -> CipherValue;

auto matmul_aa(FluxBuilder &builder, CipherValue x, CipherValue y) -> CipherValue;

auto and_bb(FluxBuilder &builder, CipherValue x, CipherValue y) -> CipherValue;

auto xor_bb(FluxBuilder &builder, CipherValue x, CipherValue y) -> CipherValue;

} // namespace fastmpc::flux::aby3