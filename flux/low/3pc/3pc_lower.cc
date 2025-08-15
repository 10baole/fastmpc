#include "fastmpc/flux/low/3pc/3pc_lower.h"
#include "fastmpc/abp/dialect/abp_ops.h"
#include "fastmpc/abp/dialect/abp_types.h"
#include "fastmpc/flux/dialect/flux_builder.h"
#include "fastmpc/flux/low/3pc/3pc_value.h"
#include "fastmpc/flux/low/3pc/function/3pc_binary.h"
#include "fastmpc/flux/low/3pc/function/3pc_io.h"
#include "fastmpc/flux/low/3pc/function/3pc_unary.h"
#include "fastmpc/ir_base/attribute.h"
#include <cassert>

using namespace std;

namespace fastmpc::flux::_3pc {

    void _3PCLower::run() {
        for (size_t i = 0; i < abp_context_->ops_size(); i++) {
            abp::OpHandle handle(i);
            abp_context_->visit(handle, *this);
        }
    }

    void _3PCLower::push(abp::OpHandle handle, PlainValue value) {
        size_t offset = plain_values_.size();
        plain_values_.push_back(value);
        map_.emplace(handle, Value{ValueKind::kPlainValue, offset});
    }

    void _3PCLower::push(abp::OpHandle handle, CipherValue value) {
        size_t offset = cipher_values_.size();
        cipher_values_.push_back(value);
        map_.emplace(handle, Value{ValueKind::kCipherValue, offset});
    }

    auto _3PCLower::get_cipher_value(abp::OpHandle handle) -> CipherValue {
        auto value = map_.find(handle)->second;
        assert(value.kind == ValueKind::kCipherValue);
        return cast(cipher_values_[value.offset]);
    }

    auto _3PCLower::get_plain_value(abp::OpHandle handle) -> PlainValue {
        auto value = map_.find(handle)->second;
        assert(value.kind == ValueKind::kPlainValue);
        return plain_values_[value.offset];
    }

    void _3PCLower::visit_value(abp::OpHandle handle, ValueVisitor &visitor) {
        auto value = map_.find(handle)->second;
        switch (value.kind) {
            case ValueKind::kPlainValue:
                return visitor.visit(cast(plain_values_[value.offset]));
            case ValueKind::kCipherValue:
                return visitor.visit(cast(cipher_values_[value.offset]));
        }
    }

    void _3PCLower::set_value(abp::OpHandle handle, CipherValue value) {
        push(handle, cast(value));
    }

    void _3PCLower::set_value(abp::OpHandle handle, PlainValue  value) {
        push(handle, cast(value));
    }

    void _3PCLower::operator()(abp::OpHandle handle, abp::InputOp op) {
        auto shape = push_shape(handle);
        if (op.type.kind == abp::TypeKind::kFixed64) {
            auto result = input<PlainValue>(*builder_, op.input_index, shape);
            set_value(handle, result);
        } else {
            auto result = input<CipherValue>(*builder_, op.input_index, shape);
            set_value(handle, result);
        }
    }

    void _3PCLower::operator()(abp::OpHandle handle, abp::OutputOp op) {
        auto type = abp_context_->type(op.operand);
        if (type.kind == abp::TypeKind::kFixed64) {
            auto value = get_plain_value(op.operand);
            output(*builder_, op.output_index, value);
        } else {
            auto value = get_cipher_value(op.operand);
            output(*builder_, op.output_index, value);
        }
    }

    void _3PCLower::operator()(abp::OpHandle handle, abp::ConstantOp op) {
        auto value = abp_context_->dense_value(op.value);
        auto shape = abp_context_->shape(op.type.shape);

        auto value_handle = builder_->push(~value);
        auto shape_handle = builder_->push(~shape);

        auto result = PlainValue{
            .p0_v = builder_->constant(value_handle, {0, shape_handle}),
            .p1_v = builder_->constant(value_handle, {1, shape_handle}),
            .p2_v = builder_->constant(value_handle, {2, shape_handle}),
        };

        set_value(handle, result);
    }

    void _3PCLower::operator()(abp::OpHandle handle, abp::NegateAOp op) {
        auto operand = get_cipher_value(op.operand);
        auto result  = negate_a(*builder_, operand);
        set_value(handle, result);
    }

    void _3PCLower::operator()(abp::OpHandle handle, abp::NegatePOp op) {
        auto operand = get_plain_value(op.operand);
        auto result  = negate_p(*builder_, operand);
        set_value(handle, result);
    }

    void _3PCLower::operator()(abp::OpHandle handle, abp::InverseOp op) {
        auto operand = get_plain_value(op.operand);
        auto result  = inverse(*builder_, operand, op.type.fixed_point);
        set_value(handle, result);
    }

    void _3PCLower::operator()(abp::OpHandle handle, abp::NotBOp op) {
        auto operand = get_cipher_value(op.operand);
        auto result  = not_b(*builder_, operand);
        set_value(handle, result);
    }

    void _3PCLower::operator()(abp::OpHandle handle, abp::BitReverseOp op) {
        auto operand = get_cipher_value(op.operand);
        auto result  = bit_reverse(*builder_, operand);
        set_value(handle, result);
    }

    void _3PCLower::operator()(abp::OpHandle handle, abp::ShiftRightOp op) {
        auto operand = get_cipher_value(op.operand);
        auto result  = shift_right(*builder_, operand, op.bits);
        set_value(handle, result);
    }

    void _3PCLower::operator()(abp::OpHandle handle, abp::TruncateAOp op) {
        auto operand = get_cipher_value(op.operand);
        auto result = truncate_a(*builder_, operand, op.bits);
        set_value(handle, result);
    }

    void _3PCLower::operator()(abp::OpHandle handle, abp::TruncatePOp op) {
        auto operand = get_plain_value(op.operand);
        auto result  = truncate_p(*builder_, operand, op.bits);
        set_value(handle, result);
    }

    void _3PCLower::operator()(abp::OpHandle handle, abp::P2AOp op) {
        auto operand = get_plain_value(op.operand);
        auto result  = p2a(*builder_, operand);
        set_value(handle, result);
    }

    void _3PCLower::operator()(abp::OpHandle handle, abp::A2BOp op) {
        auto operand = get_cipher_value(op.operand);
        auto result  = a2b(*builder_, operand);
        set_value(handle, result);
    }

    void _3PCLower::operator()(abp::OpHandle handle, abp::B2AOp op) {
        auto operand = get_cipher_value(op.operand);
        auto result  = b2a(*builder_, operand);
        set_value(handle, result);
    }

    void _3PCLower::operator()(abp::OpHandle handle, abp::AddAAOp op) {
        auto left   = get_cipher_value(op.left);
        auto right  = get_cipher_value(op.right);
        auto result = add_aa(*builder_, left, right);
        set_value(handle, result);
    }

    void _3PCLower::operator()(abp::OpHandle handle, abp::AddAPOp op) {
        auto left   = get_cipher_value(op.left);
        auto right  = get_plain_value(op.right);
        auto result = add_ap(*builder_, left, right);
        set_value(handle, result);
    }

    void _3PCLower::operator()(abp::OpHandle handle, abp::AddPPOp op) {
        auto left   = get_plain_value(op.left);
        auto right  = get_plain_value(op.right);
        auto result = add_pp(*builder_, left, right);
        set_value(handle, result);
    }

    void _3PCLower::operator()(abp::OpHandle handle, abp::MultiplyAAOp op) {
        auto left   = get_cipher_value(op.left);
        auto right  = get_cipher_value(op.right);
        auto result = multiply_aa(*builder_, left, right);
        set_value(handle, result);
    }

    void _3PCLower::operator()(abp::OpHandle handle, abp::MultiplyAPOp op) {
        auto left   = get_cipher_value(op.left);
        auto right  = get_plain_value(op.right);
        auto result = multiply_ap(*builder_, left, right);
        set_value(handle, result);
    }

    void _3PCLower::operator()(abp::OpHandle handle, abp::MultiplyPPOp op) {
        auto left   = get_plain_value(op.left);
        auto right  = get_plain_value(op.right);
        auto result = multiply_pp(*builder_, left, right);
        set_value(handle, result);
    }

    void _3PCLower::operator()(abp::OpHandle handle, abp::BroadcastOp op) {
        auto dimensions = abp_context_->dense_size_t(op.dimensions);
        auto shape      = abp_context_->shape(handle);

        struct Visitor: public ValueVisitor {
            Visitor(_3PCLower *lower, abp::OpHandle handle, DenseSizeT &&dimensions, Shape &&shape): 
                lower(lower), handle(handle), dimensions(~dimensions), shape(~shape) {}
            
            void visit(PlainValue operand) override {
                auto result = broadcast(*lower->builder_, operand, ~dimensions, ~shape);
                lower->set_value(handle, result);
            }

            void visit(CipherValue operand) override {
                auto result = broadcast(*lower->builder_, operand, ~dimensions, ~shape);
                lower->set_value(handle, result);
            }

            _3PCLower *lower;
            abp::OpHandle handle;
            DenseSizeT dimensions;
            Shape shape;
        };

        Visitor visitor(this, handle, ~dimensions, ~shape);
        visit_value(op.operand, visitor);
    }

    void _3PCLower::operator()(abp::OpHandle handle, abp::ReshapeOp op) {
        auto shape = abp_context_->shape(handle);
        
        struct Visitor : public ValueVisitor {
            Visitor(_3PCLower *lower, abp::OpHandle handle, Shape &&shape): 
                lower(lower), handle(handle), shape(~shape) {}
    
            void visit(PlainValue operand) override {
                auto result = reshape(*lower->builder_, operand, ~shape);
                lower->set_value(handle, result);
            }

            void visit(CipherValue operand) override {
                auto result = reshape(*lower->builder_, operand, ~shape);
                lower->set_value(handle, result);
            }

            _3PCLower *lower;
            abp::OpHandle handle;
            Shape shape;
        };

        Visitor visitor(this, handle, ~shape);
        visit_value(op.operand, visitor);
    }

    void _3PCLower::operator()(abp::OpHandle handle, abp::SliceOp op) {
        auto start  = abp_context_->dense_size_t(op.start);
        auto end    = abp_context_->dense_size_t(op.end);
        auto stride = abp_context_->dense_size_t(op.stride);
        
        struct Visitor: public ValueVisitor {
            Visitor(_3PCLower *lower, abp::OpHandle handle, DenseSizeT &&start, DenseSizeT &&end, DenseSizeT &&stride): 
                lower(lower), handle(handle), start(~start), end(~end), stride(~stride) {}
    
            void visit(PlainValue operand) override {
                auto result = slice(*lower->builder_, operand, ~start, ~end, ~stride);
                lower->set_value(handle, result);
            }

            void visit(CipherValue operand) override {
                auto result = slice(*lower->builder_, operand, ~start, ~end, ~stride);
                lower->set_value(handle, result);
            }

            _3PCLower *lower;
            abp::OpHandle handle;
            DenseSizeT start, end, stride;
        };

        Visitor visitor(this, handle, ~start, ~end, ~stride);
        visit_value(op.operand, visitor);
    }

    void _3PCLower::operator()(abp::OpHandle handle, abp::TransposeOp op) {
        auto permutation = abp_context_->dense_size_t(op.permutation);
        struct Visitor : public ValueVisitor {
            Visitor(_3PCLower *lower, abp::OpHandle handle, DenseSizeT &&permutation): 
                lower(lower), handle(handle), permutation(~permutation) {}

            void visit(PlainValue operand) override {
                auto result = transpose(*lower->builder_, operand, ~permutation);
                lower->set_value(handle, result);
            }

            void visit(CipherValue operand) override {
                auto result = transpose(*lower->builder_, operand, ~permutation);
                lower->set_value(handle, result);
            }

            _3PCLower *lower;
            abp::OpHandle handle;
            DenseSizeT permutation;
        };

        Visitor visitor(this, handle, ~permutation);
        visit_value(op.operand, visitor);
    }

    void _3PCLower::operator()(abp::OpHandle handle, abp::ConcateOp op) {
        if (abp_context_->type(handle).kind == abp::TypeKind::kFixed64) {
            vector <PlainValue> plain_values;
            for (auto operand : op.operands) 
                plain_values.push_back(get_plain_value(operand));
            auto result = concat(*builder_, plain_values, op.dimension);
            set_value(handle, result);
        } else {
            vector <CipherValue> plain_values;
            for (auto operand : op.operands) 
                plain_values.push_back(get_cipher_value(operand));
            auto result = concat(*builder_, plain_values, op.dimension);
            set_value(handle, result);
        }
    }

    auto _3PCLower::push_shape(abp::OpHandle handle) -> ShapeHandle {
        auto shape = abp_context_->shape(handle);
        return builder_->push(~shape);
    }

}