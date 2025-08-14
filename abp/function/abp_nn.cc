#include "fastmpc/abp/function/abp_nn.h"
#include "fastmpc/abp/function/abp_function.h"

namespace fastmpc::abp {

auto softmax(ABPBuilder &builder, OpHandle x) -> OpHandle {

    auto &context = builder.context();
    auto x_type = context.type(x);
    auto x_shape = context.shape(x_type.shape);
    size_t rank = x_shape.size();
    size_t reduce_dim = rank - 1;

    auto max_init = constant(builder, -INT16_MAX);
    Shape reduced_shape = x_shape;
    reduced_shape[reduce_dim] = 1;
    auto max_val = reduce(builder, x, max_init, {reduce_dim}, {~reduced_shape}, max);
    
    auto max_val_bcast = builder.broadcast(max_val, {reduce_dim}, x_type.shape);
    auto x_shifted = subtract(builder, x, max_val_bcast);

    // 3. Compute exponentiation
    auto exp_x = exp(builder, x_shifted);

    // 4. Sum the exponentiated values
    auto sum_init = constant(builder, 0.0f);
    auto sum_exp = reduce(builder, exp_x, sum_init, {reduce_dim}, {~reduced_shape}, add);

    // 5. Broadcast sum and divide
    auto sum_exp_bcast = builder.broadcast(sum_exp, {reduce_dim}, x_type.shape);
    auto result = divide(builder, exp_x, sum_exp_bcast);

    return result;
}

auto gelu(ABPBuilder &builder, OpHandle x) -> OpHandle {
    // Using the common and MPC-friendly approximation:
    // GeLU(x) ≈ x * sigmoid(1.702 * x) = x / (1 + exp(-1.702 * x))

    // 1. Compute the argument for exp: -1.702 * x
    auto c1 = constant(builder, -1.702f);
    auto arg = multiply(builder, c1, x);

    // 2. Compute exp
    auto exp_val = exp(builder, arg);

    // 3. Compute 1 + exp(...)
    auto c2 = constant(builder, 1.0f);
    auto denominator = add(builder, c2, exp_val);

    // 4. Compute x / (1 + exp(...))
    return divide(builder, x, denominator);
}

} // namespace fastmpc::abp