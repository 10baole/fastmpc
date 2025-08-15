#include "fastmpc/abp/function/abp_compare.h"
#include "fastmpc/abp/function/abp_binary.h"
#include "fastmpc/abp/function/abp_unary.h"

namespace fastmpc::abp {

    namespace {
        auto msb(ABPBuilder &builder, OpHandle operand) {
            return builder.shift_right(builder.a2b(operand), 63);
        }

        auto less_b(ABPBuilder &builder, OpHandle left, OpHandle right) -> OpHandle {
            return msb(builder, subtract(builder, left, right));
        }

        auto greater_b(ABPBuilder &builder, OpHandle left, OpHandle right) -> OpHandle {
            return less_b(builder, right, left);
        }
    }

    auto isEqual(ABPBuilder &builder, OpHandle left, OpHandle right) -> OpHandle {
        auto not_less    = builder.not_b(less_b(builder, left, right));
        auto not_greater = builder.not_b(greater_b(builder, left, right));
        auto result_b    = builder.and_bb(not_less, not_greater);
        return builder.b2a(result_b, 0);
    }

    auto isLess(ABPBuilder &builder, OpHandle left, OpHandle right) -> OpHandle {
        return builder.b2a(less_b(builder, left, right), 0);
    }

    auto isGreater(ABPBuilder &builder, OpHandle left, OpHandle right) -> OpHandle {
        return builder.b2a(greater_b(builder, left, right), 0);
    }

    auto isGEQ(ABPBuilder &builder, OpHandle left, OpHandle right) -> OpHandle {
        auto not_less = builder.not_b(less_b(builder, left, right));
        return builder.b2a(not_less, 0);
    }

    auto selectOne(ABPBuilder &builder, OpHandle which, OpHandle left, OpHandle right) -> OpHandle {
        auto diff       = subtract(builder, left, right);
        auto false_path = multiply(builder, diff, which);
        return add(builder, right, false_path);
    }

    auto maximize(ABPBuilder &builder, OpHandle left, OpHandle right) -> OpHandle {
        auto which = isGreater(builder, left, right);
        return selectOne(builder, which, left, right);
    }

}