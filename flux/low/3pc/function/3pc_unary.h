#pragma once
#include "fastmpc/flux/dialect/flux_builder.h"
#include "fastmpc/flux/dialect/flux_ops.h"
#include "fastmpc/flux/low/3pc/3pc_value.h"
#include "fastmpc/ir_base/attribute.h"

using namespace std;

namespace fastmpc::flux::_3pc {

    auto negate_a(FluxBuilder &builder, CipherValue operand) -> CipherValue;
    auto negate_p(FluxBuilder &builder, PlainValue operand) -> PlainValue;
    
    auto truncate_a(FluxBuilder &builder, CipherValue operand, uint8_t bits) -> CipherValue;
    auto truncate_p(FluxBuilder &builder, PlainValue  operand, uint8_t bits) -> PlainValue;   

    auto inverse(FluxBuilder &builder, PlainValue operand, uint8_t fixed_point) -> PlainValue;
    auto bit_reverse(FluxBuilder &builder, CipherValue operand) -> CipherValue;
    auto shift_left(FluxBuilder &builder, CipherValue operand, uint8_t bits) -> CipherValue;
    auto shift_right(FluxBuilder &builder, CipherValue operand, uint8_t bits) -> CipherValue;
    auto not_b(FluxBuilder &builder, CipherValue operand) -> CipherValue;

    auto p2a(FluxBuilder &builder, PlainValue  operand) -> CipherValue;

    auto broadcast(FluxBuilder &builder, CipherValue operand, DenseSizeT &&dimensions, Shape &&shape) -> CipherValue;
    auto broadcast(FluxBuilder &builder, PlainValue  operand, DenseSizeT &&dimensions, Shape &&shape) -> PlainValue;
    
    auto reshape(FluxBuilder &builder, CipherValue operand, Shape &&shape) -> CipherValue;
    auto reshape(FluxBuilder &builder, PlainValue  operand, Shape &&shape) -> PlainValue;
    
    auto slice(FluxBuilder &builder, CipherValue operand, DenseSizeT &&start, DenseSizeT &&end, DenseSizeT &&stride) -> CipherValue;
    auto slice(FluxBuilder &builder, PlainValue  operand, DenseSizeT &&start, DenseSizeT &&end, DenseSizeT &&stride) -> PlainValue;
    
    auto transpose(FluxBuilder &builder, CipherValue operand, DenseSizeT &&permutation) -> CipherValue;
    auto transpose(FluxBuilder &builder, PlainValue  operand, DenseSizeT &&permutation) -> PlainValue;
    
    auto concat(FluxBuilder &builder, vector <PlainValue>  &operands, size_t dimension) -> PlainValue;
    auto concat(FluxBuilder &builder, vector <CipherValue> &operands, size_t dimension) -> CipherValue;

}