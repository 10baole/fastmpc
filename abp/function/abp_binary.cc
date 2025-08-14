#include "fastmpc/abp/function/abp_binary.h"
#include "fastmpc/abp/function/abp_unary.h"
#include <cstdlib>

using namespace std;

namespace fastmpc::abp {
    auto add(ABPBuilder &builder, OpHandle left, OpHandle right) -> OpHandle {
        auto &context = builder.context();
        auto left_kind  = context.type(left).kind;
        auto right_kind = context.type(right).kind;
        
        switch (encode(left_kind, right_kind)) {
            case encode(TypeKind::kArithFixed64, TypeKind::kArithFixed64):
                return builder.add_aa(left, right);
            case encode(TypeKind::kArithFixed64, TypeKind::kFixed64):
                return builder.add_ap(left, right);
            case encode(TypeKind::kFixed64, TypeKind::kArithFixed64):
                return builder.add_ap(right, left);
            case encode(TypeKind::kFixed64, TypeKind::kFixed64):
                return builder.add_pp(left, right);
            default: abort();
        }
    }

    auto subtract(ABPBuilder &builder, OpHandle left, OpHandle right) -> OpHandle {
        return add(builder, left, negate(builder, right));
    }

    namespace unsafe {
        auto multiply(ABPBuilder &builder, OpHandle left, OpHandle right) {
            auto &context   = builder.context();
            auto left_kind  = context.type(left).kind;
            auto right_kind = context.type(right).kind;

            switch (encode(left_kind, right_kind)) {
                case encode(TypeKind::kArithFixed64, TypeKind::kArithFixed64):
                    return builder.multiply_aa(left, right);
                case encode(TypeKind::kArithFixed64, TypeKind::kFixed64):
                    return builder.multiply_ap(left, right);
                case encode(TypeKind::kFixed64, TypeKind::kArithFixed64):
                    return builder.multiply_ap(right, left);
                case encode(TypeKind::kFixed64, TypeKind::kFixed64):
                    return builder.multiply_pp(left, right);
                default: abort();
            }
        }

        auto dot_general(ABPBuilder &builder, OpHandle left, OpHandle right) {
            auto &context   = builder.context();
            auto left_kind  = context.type(left).kind;
            auto right_kind = context.type(right).kind;

            switch (encode(left_kind, right_kind)) {
                case encode(TypeKind::kArithFixed64, TypeKind::kArithFixed64):
                    return builder.dot_general_aa(left, right);
                default: abort();
            }
        }
    }

    auto multiply(ABPBuilder &builder, OpHandle left, OpHandle right) -> OpHandle {
        auto &context = builder.context();
        auto result = unsafe::multiply(builder, left, right);

        uint8_t fixed_point = context.type(result).fixed_point;
        if (fixed_point > builder.fixed_point()) {
            uint8_t bits = fixed_point - builder.fixed_point();
            result = truncate(builder, result, bits);
        }

        return result;
    }

    auto dot_general(ABPBuilder &builder, OpHandle left, OpHandle right) -> OpHandle {
        auto &context = builder.context();
        auto result = unsafe::dot_general(builder, left, right);

        uint8_t fixed_point = context.type(result).fixed_point;
        if (fixed_point > builder.fixed_point()) {
          uint8_t bits = fixed_point - builder.fixed_point();
          result = truncate(builder, result, bits);
        }
        
        return result;
    }

    auto bitwise_xor(ABPBuilder &builder, OpHandle left, OpHandle right) -> OpHandle {
        auto &context   = builder.context();
        auto left_kind  = context.type(left).kind;
        auto right_kind = context.type(right).kind;
        
        switch (encode(left_kind, right_kind)) {
            case encode(TypeKind::kBitArray64, TypeKind::kBitArray64):
                return builder.xor_bb(left, right);
            default: abort();
        }
    }

    auto bitwise_or(ABPBuilder &builder, OpHandle left, OpHandle right) -> OpHandle {
        auto &context   = builder.context();
        auto left_kind  = context.type(left).kind;
        auto right_kind = context.type(right).kind;

        switch (encode(left_kind, right_kind)) {
            case encode(TypeKind::kBitArray64, TypeKind::kBitArray64):
                return builder.xor_bb(left, builder.xor_bb(right, builder.and_bb(left, right)));
            default: abort();
        }
    }
}