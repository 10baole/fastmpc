#pragma once
#include "fastmpc/flux/dialect/flux_builder.h"
#include "fastmpc/flux/low/3pc/3pc_value.h"

namespace fastmpc::flux::_3pc {

    auto add_aa(FluxBuilder &builder, CipherValue x, CipherValue y) -> CipherValue;
    auto add_ap(FluxBuilder &builder, CipherValue x, PlainValue  y) -> CipherValue;
    auto add_pp(FluxBuilder &builder, PlainValue  x, PlainValue  y) -> PlainValue;

    auto multiply_aa(FluxBuilder &builder, CipherValue x, CipherValue y) -> CipherValue;
    auto multiply_ap(FluxBuilder &builder, CipherValue x, PlainValue  y) -> CipherValue;
    auto multiply_pp(FluxBuilder &builder,  PlainValue x, PlainValue  y) -> PlainValue;

}