#include "gtest/gtest.h"
#include <initializer_list>
#include <map>
#include <variant>

#include "absl/types/span.h"
#include "fastmpc/abp/dialect/abp_ops.h"
#include "fastmpc/eager/tensor.h"
#include "fastmpc/eager/tensor_ops.h"
#include "fastmpc/flux/dialect/flux_builder.h"
#include "fastmpc/flux/dialect/flux_context.h"
#include "fastmpc/flux/dialect/flux_ops.h"
#include "fastmpc/flux/executor/flux_executor.h"
#include "fastmpc/flux/low/3pc/3pc_value.h"
#include "fastmpc/flux/low/3pc/function/3pc_binary.h"
#include "fastmpc/flux/low/3pc/function/3pc_io.h"
#include "fastmpc/flux/low/3pc/function/3pc_unary.h"

namespace fastmpc::flux::_3pc::testing {

namespace {

auto as_shape(absl::Span<const size_t> span) {
  return Shape(span.begin(), span.end());
}

} // namespace

class _3PCFunctionTest : public ::testing::Test {
public:
  _3PCFunctionTest() : builder(context), executor(context) {}

  auto input_public(size_t input_index, eager::Tensor tensor) {
    executor.input(input_index, 0) = tensor;
    auto shape = builder.push(as_shape(tensor.shape()));
    return input<PlainValue>(builder, input_index, shape);
  }

  auto input_secret(size_t intput_index, eager::Tensor tensor) {
    auto zero = eager::Tensor::with_shape(tensor.shape());
    std::fill_n(zero.data(), zero.num_elements(), 0);

    executor.input(intput_index, 0) = tensor;
    executor.input(intput_index, 1) = zero;
    executor.input(intput_index, 2) = zero;

    auto shape = builder.push(as_shape(tensor.shape()));
    return input<CipherValue>(builder, intput_index, shape);
  }

  auto output_public(size_t output_index) {
    return executor.output(output_index, 0);
  }

  auto output_secret(size_t output_index) {
    auto x0 = executor.output(output_index, 0);
    auto x1 = executor.output(output_index, 1);
    auto x2 = executor.output(output_index, 2);
    return eager::add(x0, eager::add(x1, x2));
  }

protected:
  FluxContext context;
  FluxBuilder builder;
  FluxExecutor executor;
};

namespace {

auto make_tensor(std::initializer_list<uint64_t> list) {
  auto tensor = eager::Tensor::with_shape({list.size()});
  std::copy(list.begin(), list.end(), tensor.data());
  return tensor;
}

} // namespace

TEST_F(_3PCFunctionTest, test_io_public) {
  auto x = input_public(0, make_tensor({114, 514}));
  output(builder, 0, x);
  executor.run();
  auto result = output_public(0);
  EXPECT_EQ(result.at({0}), 114);
  EXPECT_EQ(result.at({1}), 514);
}

TEST_F(_3PCFunctionTest, test_io_secret) {
  auto x = input_secret(0, make_tensor({114, 514}));
  output(builder, 0, x);
  executor.run();
  auto result = output_secret(0);
  EXPECT_EQ(result.at({0}), 114);
  EXPECT_EQ(result.at({1}), 514);
}

TEST_F(_3PCFunctionTest, add_ap) {
  auto a = input_secret(0, make_tensor({114, 514}));
  auto p = input_public(1, make_tensor({1919, 810}));
  auto result = add_ap(builder, a, p);
  output(builder, 0, result);
  executor.run();
  {
    auto result = output_secret(0);
    EXPECT_EQ(result.at({0}), 114 + 1919);
    EXPECT_EQ(result.at({1}), 514 + 810);
  }
}

TEST_F(_3PCFunctionTest, add_pp) {
  auto x = input_public(0, make_tensor({114, 514}));
  auto y = input_public(1, make_tensor({1919, 810}));
  auto result = add_pp(builder, x, y);
  output(builder, 0, result);
  executor.run();
  {
    auto result = output_public(0);
    EXPECT_EQ(result.at({0}), 114 + 1919);
    EXPECT_EQ(result.at({1}), 514 + 810);
  }
}

TEST_F(_3PCFunctionTest, multiply_ap) {
  auto a = input_secret(0, make_tensor({114, 514}));
  auto p = input_public(1, make_tensor({1919, 810}));
  auto result = multiply_ap(builder, a, p);
  output(builder, 0, result);
  executor.run();
  {
    auto result = output_secret(0);
    EXPECT_EQ(result.at({0}), 114 * 1919);
    EXPECT_EQ(result.at({1}), 514 * 810);
  }
}

TEST_F(_3PCFunctionTest, multiply_pp) {
  auto x = input_public(0, make_tensor({114, 514}));
  auto y = input_public(1, make_tensor({1919, 810}));
  auto result = multiply_pp(builder, x, y);
  output(builder, 0, result);
  executor.run();
  {
    auto result = output_public(0);
    EXPECT_EQ(result.at({0}), 114 * 1919);
    EXPECT_EQ(result.at({1}), 514 * 810);
  }
}

TEST_F(_3PCFunctionTest, negate_a) {
  auto x = input_secret(0, make_tensor({114, 514}));
  auto result = negate_a(builder, x);
  output(builder, 0, result);
  executor.run();
  {
    auto result = output_public(0);
    EXPECT_EQ(result.at({0}), static_cast<uint64_t>(-114));
    EXPECT_EQ(result.at({1}), static_cast<uint64_t>(-514));
  }
}

TEST_F(_3PCFunctionTest, negate_p) {
  auto x = input_public(0, make_tensor({114, 514}));
  auto result = negate_p(builder, x);
  output(builder, 0, result);
  executor.run();
  {
    auto result = output_public(0);
    EXPECT_EQ(result.at({0}), static_cast<uint64_t>(-114));
    EXPECT_EQ(result.at({1}), static_cast<uint64_t>(-514));
  }
}

TEST_F(_3PCFunctionTest, inverse) {
  auto x = input_public(0, make_tensor({114 * 1024, 514 * 1024}));
  auto result = inverse(builder, x, 10);
  output(builder, 0, result);
  executor.run();
  {
    auto result = output_public(0);
    EXPECT_EQ(result.at({0}), 1024 / 114);
    EXPECT_EQ(result.at({1}), 1024 / 514);
  }
}

TEST_F(_3PCFunctionTest, bit_reverse) {
  auto x = input_secret(0, make_tensor({114, 514}));
  auto result = bit_reverse(builder, x);
  output(builder, 0, result);
  executor.run();
  {
    auto result = output_public(0);
    EXPECT_EQ(result.at({0}), __builtin_bitreverse64(114));
    EXPECT_EQ(result.at({1}), __builtin_bitreverse64(514));
  }
}

TEST_F(_3PCFunctionTest, truncate_p) {
  auto x = input_public(0, make_tensor({114 * 1024, 514 * 1024}));
  auto result = truncate_p(builder, x, 10);
  output(builder, 0, result);
  executor.run();
  {
    auto result = output_public(0);
    EXPECT_EQ(result.at({0}), 114);
    EXPECT_EQ(result.at({1}), 514);
  }
}

TEST_F(_3PCFunctionTest, p2a) {
  auto x = input_public(0, make_tensor({114, 514}));
  auto result = p2a(builder, x);
  output(builder, 0, result);
  executor.run();
  {
    auto result = output_secret(0);
    EXPECT_EQ(result.at({0}), 114);
    EXPECT_EQ(result.at({1}), 514);
  }
}

} // namespace fastmpc::flux::_3pc::testing