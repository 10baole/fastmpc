#include "gtest/gtest.h"

#include <cmath>
#include <vector>

#include "fastmpc/abp/dialect/abp_types.h"
#include "fastmpc/abp/executor/abp_executor.h"
#include "fastmpc/abp/function/abp_function.h"
#include "fastmpc/eager/tensor.h"
#include "fastmpc/ir_base/attribute.h"

using namespace fastmpc::abp;
using namespace fastmpc;

namespace {

class Environment {
public:
  explicit Environment(ABPBuilder &builder, ABPExecutor &executor)
      : builder_(&builder), executor_(&executor) {}

  auto arg_float(size_t index, float value, bool is_public) -> OpHandle {
    executor_->input(index) = encode(value);
    Type type{
        .kind = is_public ? TypeKind::kFixed64 : TypeKind::kArithFixed64,
        .fixed_point = builder_->fixed_point(),
        .shape = builder_->push(Shape{}),
    };
    return builder_->input(index, type);
  }

  auto arg_int(size_t index, uint64_t value, bool is_public) -> OpHandle {
    eager::Tensor tensor;
    *tensor.data() = value;
    executor_->input(index) = tensor;
    Type type{
        .kind = is_public ? TypeKind::kFixed64 : TypeKind::kArithFixed64,
        .fixed_point = 0,
        .shape = builder_->push(Shape{}),
    };
    return builder_->input(index, type);
  }

  auto output_float(size_t index = 0) {
    return decode(executor_->output(index).data()[0]);
  }

  auto output_int(size_t index = 0) { return executor_->output(index).at({}); }

private:
  auto encode(float value) -> eager::Tensor {
    eager::Tensor tensor;
    int scalar = 1 << builder_->fixed_point();
    *tensor.data() =
        static_cast<uint64_t>(static_cast<int64_t>(value * scalar));
    return tensor;
  }

  auto decode(uint64_t value) -> float {
    int scalar = 1 << builder_->fixed_point();
    return static_cast<float>(static_cast<int64_t>(value)) / scalar;
  }

  ABPBuilder *builder_;
  ABPExecutor *executor_;
};

} // namespace

#define SETUP(FIXED_POINT, INPUT_SIZE, OUTPUT_SIZE)                            \
  ABPContext context;                                                          \
  ABPBuilder builder(context, FIXED_POINT);                                    \
  ABPExecutor executor(context, INPUT_SIZE, OUTPUT_SIZE);                      \
  Environment env(builder, executor)

TEST(abp_function_test, add_pa) {
  SETUP(15, 2, 1);
  auto left = env.arg_float(0, -0.5, true);
  auto right = env.arg_float(1, -1.5, false);
  auto result = add(builder, left, right);
  builder.output(result, 0);
  executor.run();
  EXPECT_EQ(env.output_float(), -2);
}

TEST(abp_function_test, subtract_pp) {
  SETUP(15, 2, 1);
  auto left = env.arg_float(0, -0.5, true);
  auto right = env.arg_float(1, 1.5, true);
  auto result = subtract(builder, left, right);
  builder.output(result, 0);
  executor.run();
  EXPECT_EQ(env.output_float(), -2);
}

TEST(abp_function_test, euqal_true) {
  SETUP(15, 2, 1);
  auto left = env.arg_float(0, 0.5f, false);
  auto right = env.arg_float(1, 0.5f, false);
  auto result = isEqual(builder, left, right);
  builder.output(result, 0);
  executor.run();
  EXPECT_EQ(env.output_int(), 1);
}

TEST(abp_function_test, euqal_false) {
  SETUP(15, 2, 1);
  auto left = env.arg_float(0, 0.5f, false);
  auto right = env.arg_float(1, -0.5f, false);
  auto result = isEqual(builder, left, right);
  builder.output(result, 0);
  executor.run();
  EXPECT_EQ(env.output_int(), 0);
}

TEST(abp_function_test, less_less) {
  SETUP(15, 2, 1);
  auto left = env.arg_float(0, 0.5f, false);
  auto right = env.arg_float(1, 1.f, false);
  auto result = isLess(builder, left, right);
  builder.output(result, 0);
  executor.run();
  EXPECT_EQ(env.output_int(), 1);
}

TEST(abp_function_test, less_equal) {
  SETUP(15, 2, 1);
  auto left = env.arg_float(0, 0.5f, false);
  auto right = env.arg_float(1, 0.5f, false);
  auto result = isLess(builder, left, right);
  builder.output(result, 0);
  executor.run();
  EXPECT_EQ(env.output_int(), 0);
}

TEST(abp_function_test, less_greater) {
  SETUP(15, 2, 1);
  auto left = env.arg_float(0, 0.7f, false);
  auto right = env.arg_float(1, 0.6f, false);
  auto result = isLess(builder, left, right);
  builder.output(result, 0);
  executor.run();
  EXPECT_EQ(env.output_int(), 0);
}

TEST(abp_function_test, greater_greater) {
  SETUP(15, 2, 1);
  auto left = env.arg_float(0, 0.7f, false);
  auto right = env.arg_float(1, 0.6f, false);
  auto result = isGreater(builder, left, right);
  builder.output(result, 0);
  executor.run();
  EXPECT_EQ(env.output_int(), 1);
}

TEST(abp_function_test, greater_equal) {
  SETUP(15, 2, 1);
  auto left = env.arg_float(0, 0.5f, false);
  auto right = env.arg_float(1, 0.5f, false);
  auto result = isGreater(builder, left, right);
  builder.output(result, 0);
  executor.run();
  EXPECT_EQ(env.output_int(), 0);
}

TEST(abp_function_test, greater_less) {
  SETUP(15, 2, 1);
  auto left = env.arg_float(0, 0.5f, false);
  auto right = env.arg_float(1, 1.f, false);
  auto result = isGreater(builder, left, right);
  builder.output(result, 0);
  executor.run();
  EXPECT_EQ(env.output_int(), 0);
}

TEST(abp_function_test, greater_equal_less) {
  SETUP(15, 2, 1);
  auto left = env.arg_float(0, 0.5f, false);
  auto right = env.arg_float(1, 1.f, false);
  auto result = isGEQ(builder, left, right);
  builder.output(result, 0);
  executor.run();
  EXPECT_EQ(env.output_int(), 0);
}

TEST(abp_function_test, greater_equal_equal) {
  SETUP(15, 2, 1);
  auto left = env.arg_float(0, 0.5f, false);
  auto right = env.arg_float(1, 0.5f, false);
  auto result = isGEQ(builder, left, right);
  builder.output(result, 0);
  executor.run();
  EXPECT_EQ(env.output_int(), 1);
}

TEST(abp_function_test, greater_equal_greater) {
  SETUP(15, 2, 1);
  auto left = env.arg_float(0, 0.5f, false);
  auto right = env.arg_float(1, 0.1f, false);
  auto result = isGEQ(builder, left, right);
  builder.output(result, 0);
  executor.run();
  EXPECT_EQ(env.output_int(), 1);
}

TEST(abp_function_test, select_true) {
  SETUP(15, 3, 1);
  auto which = env.arg_int(0, 1, false);
  auto left = env.arg_float(1, 10000.5f, false);
  auto right = env.arg_float(2, 100.5f, false);
  auto result = selectOne(builder, which, left, right);
  builder.output(result, 0);
  executor.run();
  EXPECT_EQ(env.output_float(), 10000.5);
}

TEST(abp_function_test, select_false) {
  SETUP(15, 3, 1);
  auto which = env.arg_int(0, 0, false);
  auto left = env.arg_float(1, 10000.5f, false);
  auto right = env.arg_float(2, 100.5f, false);
  auto result = selectOne(builder, which, left, right);
  builder.output(result, 0);
  executor.run();
  EXPECT_EQ(env.output_float(), 100.5);
}

TEST(abp_function_test, max_left) {
  SETUP(15, 2, 1);
  auto left = env.arg_float(0, 0.5f, false);
  auto right = env.arg_float(1, 0.1f, false);
  auto result = maximize(builder, left, right);
  builder.output(result, 0);
  executor.run();
  EXPECT_EQ(env.output_float(), 0.5);
}

TEST(abp_function_test, max_right) {
  SETUP(15, 2, 1);
  auto left = env.arg_float(0, 0.5f, false);
  auto right = env.arg_float(1, 1.5f, false);
  auto result = maximize(builder, left, right);
  builder.output(result, 0);
  executor.run();
  EXPECT_EQ(env.output_float(), 1.5);
}

TEST(abp_function_test, or) {
  SETUP(15, 2, 1);
  auto left = env.arg_int(0, 114ul, false);
  auto right = env.arg_int(1, 514ul, false);
  left = a2b(builder, left);
  right = a2b(builder, right);
  auto result = bitwise_or(builder, left, right);
  result = b2a(builder, result, 0);
  builder.output(result, 0);
  executor.run();
  EXPECT_EQ(env.output_int(), 114 | 514);
}

TEST(abp_function_test, divide_pp) {
  SETUP(15, 2, 1);
  auto left = env.arg_float(0, 0.5f, false);
  auto right = env.arg_float(1, -0.25f, false);
  auto result = divide(builder, left, right);
  builder.output(result, 0);
  executor.run();
  EXPECT_NEAR(env.output_float(), -2, 1e-4);
}

TEST(abp_function_test, divide_ap) {
  SETUP(15, 2, 1);
  auto left = env.arg_float(0, 5.f, true);
  auto right = env.arg_float(1, -0.25f, false);
  auto result = divide(builder, left, right);
  builder.output(result, 0);
  executor.run();
  EXPECT_NEAR(env.output_float(), -20, 1e-3);
}

TEST(abp_function_test, divide_pa) {
  SETUP(15, 2, 1);
  auto left = env.arg_float(0, -0.5f, false);
  auto right = env.arg_float(1, -3.25f, true);
  auto result = divide(builder, left, right);
  builder.output(result, 0);
  executor.run();
  EXPECT_NEAR(env.output_float(), 0.15384615384615385, 1e-4);
}

TEST(abp_function_test, exp) {
  SETUP(18, 1, 1);
  auto operand = env.arg_float(0, 0.5f, false);
  auto result = exp(builder, operand);
  builder.output(result, 0);
  executor.run();
  EXPECT_NEAR(env.output_float(), std::exp(0.5), 1e-3);
}

TEST(abp_function_test, log2) {
  SETUP(15, 1, 1);
  auto operand = env.arg_float(0, 1.75f, false);
  auto result = log2(builder, operand);
  builder.output(result, 0);
  executor.run();
  EXPECT_NEAR(env.output_float(), std::log2(1.75), 1e-3);
}

TEST(abp_function_test, log) {
  SETUP(15, 1, 1);
  const float e = std::exp(3.1415826);
  auto operand = env.arg_float(0, e, false);
  auto result = log(builder, operand);
  builder.output(result, 0);
  executor.run();
  EXPECT_NEAR(env.output_float(), 3.1415826, 1e-3);
}

TEST(abp_function_test, reduce_1) {
  ABPContext context;
  ABPBuilder builder(context, 15);
  ABPExecutor executor(context, 2, 1);

  const size_t size = 100;

  auto input_tensor = eager::Tensor::with_shape({size});
  for (size_t i = 0; i < size; i++) {
    input_tensor.data()[i] = i;
  }
  auto init_tensor = eager::Tensor::with_shape({});
  init_tensor.data()[0] = 0;

  executor.input(0) = input_tensor;
  executor.input(1) = init_tensor;

  auto input_type = Type{
      .kind = TypeKind::kArithFixed64,
      .fixed_point = 0,
      .shape = builder.push(Shape{size}),
  };

  auto init_type = Type{
      .kind = TypeKind::kArithFixed64,
      .fixed_point = 0,
      .shape = builder.push(Shape{}),
  };

  auto operand = builder.input(0, input_type);
  auto init = builder.input(1, init_type);
  auto result = reduce(builder, operand, init, {0}, {}, add);
  builder.output(result, 0);

  executor.run();
  auto output = executor.output(0);

  // context.print(std::cerr, &executor);

  Shape expect_shape{};
  EXPECT_EQ(output.shape(), expect_shape);
  EXPECT_EQ(output.data()[0], 4950);
}