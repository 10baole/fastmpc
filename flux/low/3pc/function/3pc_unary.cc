#include "fastmpc/flux/low/3pc/function/3pc_unary.h"
#include "fastmpc/flux/dialect/flux_builder.h"
#include "fastmpc/flux/dialect/flux_ops.h"
#include "fastmpc/flux/low/3pc/3pc_value.h"

using namespace std;

namespace fastmpc::flux::_3pc {

    auto negate_p(FluxBuilder &builder, PlainValue operand) -> PlainValue {
        auto [p0_v, p1_v, p2_v] = operand;
        return PlainValue{
            .p0_v = builder.negate(p0_v),
            .p1_v = builder.negate(p1_v),
            .p2_v = builder.negate(p2_v),
        };
    }

    auto negate_a(FluxBuilder &builder, CipherValue operand) -> CipherValue {
        auto [p0_v0, p0_v1, p1_v1, p1_v2, p2_v2, p2_v0] = operand;
        return CipherValue{
            .p0_v0 = builder.negate(p0_v0),
            .p0_v1 = builder.negate(p0_v1),

            .p1_v1 = builder.negate(p1_v1),
            .p1_v2 = builder.negate(p1_v2),

            .p2_v2 = builder.negate(p2_v2),
            .p2_v0 = builder.negate(p2_v0),
        };
    }

    auto inverse(FluxBuilder &builder, PlainValue operand, uint8_t fixed_point) -> PlainValue {
        auto [p0_v, p1_v, p2_v] = operand;
        return PlainValue{
            .p0_v = builder.inverse(p0_v, fixed_point),
            .p1_v = builder.inverse(p1_v, fixed_point),
            .p2_v = builder.inverse(p2_v, fixed_point),
        };
    }

    auto bit_reverse(FluxBuilder &builder, CipherValue operand) -> CipherValue {
        auto [p0_v0, p0_v1, p1_v1, p1_v2, p2_v2, p2_v0] = operand;
        return CipherValue{
            .p0_v0 = builder.bit_reverse(p0_v0),
            .p0_v1 = builder.bit_reverse(p0_v1),

            .p1_v1 = builder.bit_reverse(p1_v1),
            .p1_v2 = builder.bit_reverse(p1_v2),

            .p2_v2 = builder.bit_reverse(p2_v2),
            .p2_v0 = builder.bit_reverse(p2_v0),
        };
    }

    auto truncate_a(FluxBuilder &builder, CipherValue operand, uint8_t bits) -> CipherValue {
        auto [p0_v0, p0_v1, p1_v1, p1_v2, p2_v2, p2_v0] = operand;
        return CipherValue{
            .p0_v0 = builder.arith_shift_right(p0_v0, bits),
            .p0_v1 = builder.arith_shift_right(p0_v1, bits),

            .p1_v1 = builder.arith_shift_right(p1_v1, bits),
            .p1_v2 = builder.arith_shift_right(p1_v2, bits),

            .p2_v2 = builder.arith_shift_right(p2_v2, bits),
            .p2_v0 = builder.arith_shift_right(p2_v0, bits),
        };
    }

    auto truncate_p(FluxBuilder &builder, PlainValue operand, uint8_t bits) -> PlainValue {
        auto [p0_v, p1_v, p2_v] = operand;
        return PlainValue{
            .p0_v = builder.arith_shift_right(p0_v, bits),
            .p1_v = builder.arith_shift_right(p1_v, bits),
            .p2_v = builder.arith_shift_right(p2_v, bits),
        };
    }

    auto shift_left(FluxBuilder &builder, CipherValue operand, uint8_t bits) -> CipherValue {
        auto [p0_v0, p0_v1, p1_v1, p1_v2, p2_v2, p2_v0] = operand;
        return CipherValue{
            .p0_v0 = builder.shift_left(p0_v0, bits),
            .p0_v1 = builder.shift_left(p0_v1, bits),

            .p1_v1 = builder.shift_left(p1_v1, bits),
            .p1_v2 = builder.shift_left(p1_v2, bits),

            .p2_v2 = builder.shift_left(p2_v2, bits),
            .p2_v0 = builder.shift_left(p2_v0, bits),
        };
    }

    auto shift_right(FluxBuilder &builder, CipherValue operand, uint8_t bits) -> CipherValue {
        auto [p0_v0, p0_v1, p1_v1, p1_v2, p2_v2, p2_v0] = operand;
        return CipherValue{
            .p0_v0 = builder.logic_shift_right(p0_v0, bits),
            .p0_v1 = builder.logic_shift_right(p0_v1, bits),

            .p1_v1 = builder.logic_shift_right(p1_v1, bits),
            .p1_v2 = builder.logic_shift_right(p1_v2, bits),

            .p2_v2 = builder.logic_shift_right(p2_v2, bits),
            .p2_v0 = builder.logic_shift_right(p2_v0, bits),
        };
    }

    auto not_b(FluxBuilder &builder, CipherValue operand) -> CipherValue {
        auto [p0_v0, p0_v1, p1_v1, p1_v2, p2_v2, p2_v0] = operand;
        return CipherValue{
            .p0_v0 = builder._not(p0_v0),
            .p0_v1 = builder._not(p0_v1),

            .p1_v1 = builder._not(p1_v1),
            .p1_v2 = builder._not(p1_v2),

            .p2_v2 = builder._not(p2_v2),
            .p2_v0 = builder._not(p2_v0),
        };
    }

    auto p2a(FluxBuilder &builder, PlainValue operand) -> CipherValue {
        auto &context = builder.context();
        auto shape = context.type(operand.p0_v).shape;

        auto make_zeros = [&](size_t holder) {
            Type type{.holder = holder, .shape = builder.push(Shape{}),};
            auto result = builder.constant({0}, type);
            if (!context.shape(shape).empty()) 
                result = builder.broadcast(result, {}, shape);
        
            return result;
        };

        return CipherValue{
            .p0_v0 = make_zeros(0),
            .p0_v1 = make_zeros(0),

            .p1_v1 = make_zeros(1),
            .p1_v2 = operand.p1_v,

            .p2_v2 = operand.p2_v,
            .p2_v0 = make_zeros(2),
        };
    }

    namespace {

        template <class Func> 
        auto apply(CipherValue operand, Func &&func) {
            auto [p0_v0, p0_v1, p1_v1, p1_v2, p2_v2, p2_v0] = operand;
            return CipherValue{
                .p0_v0 = func(p0_v0),
                .p0_v1 = func(p0_v1),

                .p1_v1 = func(p1_v1),
                .p1_v2 = func(p1_v2),

                .p2_v2 = func(p2_v2),
                .p2_v0 = func(p2_v0),
            };
        }

        template <class Func> 
        auto apply(PlainValue operand, Func &&func) {
            auto [p0_v, p1_v, p2_v] = operand;
            return PlainValue{
                .p0_v = func(p0_v),
                .p1_v = func(p1_v),
                .p2_v = func(p2_v),
            };
        }

    }

    auto broadcast(FluxBuilder &builder, CipherValue operand, DenseSizeT &&dimensions, Shape &&shape) -> CipherValue {
        return apply(operand, [&builder, shape = builder.push(~shape), dimensions = builder.push(~dimensions)](OpHandle op) {
            return builder.broadcast(op, dimensions, shape);
        });
    }

    auto broadcast(FluxBuilder &builder, PlainValue operand, DenseSizeT &&dimensions, Shape &&shape) -> PlainValue {
        return apply(operand, [&builder, shape = builder.push(~shape), dimensions = builder.push(~dimensions)](OpHandle op) {
            return builder.broadcast(op, dimensions, shape);
        });
    }

    auto reshape(FluxBuilder &builder, CipherValue operand, Shape &&shape)
        -> CipherValue {
      return apply(operand, [&builder, shape = builder.push(~shape)](OpHandle op) {
        return builder.reshape(op, shape);
      });
    }

    auto reshape(FluxBuilder &builder, PlainValue operand, Shape &&shape)
        -> PlainValue {
      return apply(operand, [&builder, shape = builder.push(~shape)](OpHandle op) {
        return builder.reshape(op, shape);
      });
    }

    auto slice(FluxBuilder &builder, CipherValue operand, DenseSizeT &&start, DenseSizeT &&end, DenseSizeT &&stride) -> CipherValue {
        return apply(operand, [&builder, start = builder.push(~start), end = builder.push(~end), stride = builder.push(~stride)](OpHandle op) {
            return builder.slice(op, start, end, stride);
        });
    }

    auto slice(FluxBuilder &builder, PlainValue operand, DenseSizeT &&start, DenseSizeT &&end, DenseSizeT &&stride) -> PlainValue {
        return apply(operand, [&builder, start = builder.push(~start), end = builder.push(~end), stride = builder.push(~stride)](OpHandle op) {
            return builder.slice(op, start, end, stride);
        });
    }

    auto transpose(FluxBuilder &builder, CipherValue operand, DenseSizeT &&permutation) -> CipherValue {
        return apply(operand, [&builder, permutation = builder.push(~permutation)](OpHandle op) { 
            return builder.transpose(op, permutation); 
        });
    }

    auto transpose(FluxBuilder &builder, PlainValue operand, DenseSizeT &&permutation) -> PlainValue {
        return apply(operand,[&builder, permutation = builder.push(~permutation)](OpHandle op) { 
            return builder.transpose(op, permutation); 
        });
    }

    auto concat(FluxBuilder &builder, vector<PlainValue> &operands, size_t dimension) -> PlainValue {
        auto func = [&](OpHandle PlainValue::*p) {
            vector <OpHandle> ops;
            for (auto value : operands) 
                ops.push_back(value.*p);
            
            return builder.concate(move(ops), dimension);
        };

        return PlainValue{
            .p0_v = func(&PlainValue::p0_v),
            .p1_v = func(&PlainValue::p1_v),
            .p2_v = func(&PlainValue::p2_v),
        };
    }

    auto concat(FluxBuilder &builder, vector<CipherValue> &operands, size_t dimension) -> CipherValue {
        auto func = [&](OpHandle CipherValue::*p) {
            vector <OpHandle> ops;
            for (auto value : operands)
                ops.push_back(value.*p);
          
            return builder.concate(move(ops), dimension);
        };

        return CipherValue{
            .p0_v0 = func(&CipherValue::p0_v0),
            .p0_v1 = func(&CipherValue::p0_v1),
            .p1_v1 = func(&CipherValue::p1_v1),
            .p1_v2 = func(&CipherValue::p1_v2),
            .p2_v2 = func(&CipherValue::p2_v2),
            .p2_v0 = func(&CipherValue::p2_v0),
        };
    }

}