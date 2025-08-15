#pragma once
#include "fastmpc/abp/dialect/abp_context.h"
#include "fastmpc/abp/dialect/abp_ops.h"
#include "fastmpc/abp/dialect/abp_types.h"
#include <cstddef>

namespace fastmpc::abp {

class ABPBuilder {
    public:
        explicit ABPBuilder(ABPContext &context, uint8_t fixed_point): inner_(&context), fixed_point_(fixed_point) {}
        auto context() -> ABPContext & { return *inner_; }
        auto fixed_point() -> uint8_t { return fixed_point_; }

        auto input(size_t input_index, Type output_type) -> OpHandle;
        auto output(OpHandle operand, size_t output_index) -> OpHandle;

        auto negate_a(OpHandle operand) -> OpHandle;
        auto negate_p(OpHandle operand) -> OpHandle;
        auto inverse(OpHandle operand) -> OpHandle;

        auto add_aa(OpHandle left, OpHandle right) -> OpHandle;
        auto add_ap(OpHandle left, OpHandle right) -> OpHandle;
        auto add_pp(OpHandle left, OpHandle right) -> OpHandle;
        auto multiply_aa(OpHandle left, OpHandle right)    -> OpHandle;
        auto multiply_ap(OpHandle left, OpHandle right)    -> OpHandle;
        auto multiply_pp(OpHandle left, OpHandle right)    -> OpHandle;
        auto dot_general_aa(OpHandle left, OpHandle right) -> OpHandle;

        auto softmax(OpHandle operand, int64_t axis) -> OpHandle;
        auto gelu(OpHandle operand) -> OpHandle;
      
        auto divide_pow_of_2(OpHandle operand, uint8_t pow) -> OpHandle;
        auto truncate_a(OpHandle operand, uint8_t bits)     -> OpHandle;
        auto truncate_p(OpHandle operand, uint8_t bits)     -> OpHandle;

        auto not_b(OpHandle operand) -> OpHandle;
        auto bit_reverse(OpHandle opearnd) -> OpHandle;
        auto xor_bb(OpHandle left, OpHandle right) -> OpHandle;
        auto and_bb(OpHandle left, OpHandle right) -> OpHandle;
        auto shift_right(OpHandle operand, uint8_t bits) -> OpHandle;

        auto p2a(OpHandle operand) -> OpHandle;
        auto a2b(OpHandle operand) -> OpHandle;
        auto b2a(OpHandle operand, uint8_t fixed_point) -> OpHandle;

        auto constant(DenseValueHandle value, Type type) -> OpHandle;
        auto broadcast(OpHandle operand, DenseSizeT dimensions, ShapeHandle shape) -> OpHandle;
        auto concate(std::vector<OpHandle> &&operands, size_t dimension) -> OpHandle;
        auto reshape(OpHandle operand, ShapeHandle shape) -> OpHandle;
        auto transpose(OpHandle operand, DenseSizeT permutation) -> OpHandle;
        auto slice(OpHandle operand, DenseSizeT tart, DenseSizeT end) -> OpHandle;
        auto slice(OpHandle operand, DenseSizeT tart, DenseSizeT end, DenseSizeT stride) -> OpHandle;

        template <class T> auto push(T &&value) -> typename T::handle_type;
        private:
            template <class T> auto push_op(T &&op) -> OpHandle;
            auto multiply_result(OpHandle left, OpHandle right) -> Type;
            auto is_a(OpHandle operand) const -> bool;
            auto is_b(OpHandle operand) const -> bool;
            auto is_p(OpHandle operand) const -> bool;
            auto is_aa(OpHandle left, OpHandle right) const -> bool;
            auto is_bb(OpHandle left, OpHandle right) const -> bool;
            auto is_ap(OpHandle left, OpHandle right) const -> bool;
            auto is_pp(OpHandle left, OpHandle right) const -> bool;
            auto check_shape(OpHandle left, OpHandle right) const -> bool;
            auto check_fixed_point(OpHandle left, OpHandle right) const -> bool;

            ABPContext *inner_;
            uint8_t fixed_point_;
    };

    template <> inline 
    auto ABPBuilder::push<Shape>(Shape &&shape) -> ShapeHandle {
        return inner_->shape_list_.push(~shape);
    }

    template <> inline 
    auto ABPBuilder::push<DenseValue>(DenseValue &&value) -> DenseValueHandle {
        return inner_->dense_value_list_.push(~value);
    }

    template <> inline 
    auto ABPBuilder::push<DenseSizeT>(DenseSizeT &&dense_size_t) -> DenseSizeTHandle {
        return inner_->dense_size_t_list_.push(~dense_size_t);
    }

};