#include "fastmpc/abp/function/abp_nn.h"
#include "fastmpc/abp/function/abp_constant.h"
#include "fastmpc/abp/function/abp_reduce.h"
#include "fastmpc/abp/function/abp_binary.h"
#include "fastmpc/abp/function/abp_unary.h"
#include "fastmpc/abp/function/abp_divide.h"
#include "fastmpc/abp/function/abp_compare.h"
#include <limits>

using namespace std;

namespace fastmpc::abp {

    auto softmax(ABPBuilder &builder, OpHandle x) -> OpHandle {
        auto &context = builder.context();
        auto x_type = context.type(x);
        auto x_shape = context.shape(x_type.shape);
        size_t rank = x_shape.size();
        size_t reduce_dim = rank - 1;

        auto max_init = constant(builder, - numeric_limits<float>::infinity());
        Shape reduced_shape = x_shape;
        reduced_shape[reduce_dim] = 1;
        auto max_val = reduce(builder, x, max_init, {reduce_dim}, {~reduced_shape}, maximize);
        
        auto max_val_bcast = builder.broadcast(max_val, {reduce_dim}, x_type.shape);
        auto x_shifted = subtract(builder, x, max_val_bcast);
        auto exp_x = exp(builder, x_shifted);

        auto sum_init = constant(builder, 0.0f);
        auto sum_exp = reduce(builder, exp_x, sum_init, {reduce_dim}, {~reduced_shape}, add);

        auto sum_exp_bcast = builder.broadcast(sum_exp, {reduce_dim}, x_type.shape);
        auto result = divide(builder, exp_x, sum_exp_bcast);
        return result;
    }

    auto gelu(ABPBuilder &builder, OpHandle x) -> OpHandle {
        auto c1          = constant(builder, -1.702f);
        auto arg         = multiply(builder, c1, x);
        auto exp_val     = exp(builder, arg);
        auto c2          = constant(builder, 1.0f);
        auto denominator = add(builder, c2, exp_val);
        auto result      = divide(builder, x, denominator);
        return result;
    }

}