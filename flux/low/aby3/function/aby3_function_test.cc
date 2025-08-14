#include "gtest/gtest.h"
#include <cassert>
#include <iterator>
#include <numeric>

#include "absl/types/span.h"
#include "fastmpc/eager/tensor.h"
#include "fastmpc/eager/tensor_ops.h"
#include "fastmpc/flux/dialect/flux_builder.h"
#include "fastmpc/flux/dialect/flux_context.h"
#include "fastmpc/flux/executor/flux_executor.h"
#include "fastmpc/flux/low/3pc/function/3pc_io.h"
#include "fastmpc/flux/low/aby3/aby3_value.h"
#include "fastmpc/flux/low/aby3/function/aby3_binary.h"
#include "fastmpc/flux/low/aby3/function/aby3_casting.h"
#include "fastmpc/flux/low/aby3/function/aby3_unary.h"
#include "fastmpc/ir_base/attribute.h"

namespace fastmpc::flux::aby3::testing {

namespace {

auto as_shape(absl::Span<const size_t> span) {
  return Shape(span.begin(), span.end());
}

} // namespace

class aby3FunctionTest : public ::testing::Test {
public:
  aby3FunctionTest() : builder(context), executor(context) {}

  auto input_public(size_t input_index, eager::Tensor tensor) {
    executor.input(input_index, 0) = tensor;
    auto shape = builder.push(as_shape(tensor.shape()));
    return cast(_3pc::input<_3pc::PlainValue>(builder, input_index, shape));
  }

  auto input_secret(size_t intput_index, eager::Tensor tensor) {
    auto zero = eager::Tensor::with_shape(tensor.shape());
    std::fill_n(zero.data(), zero.num_elements(), 0);

    executor.input(intput_index, 0) = tensor;
    executor.input(intput_index, 1) = zero;
    executor.input(intput_index, 2) = zero;

    auto shape = builder.push(as_shape(tensor.shape()));
    return cast(_3pc::input<_3pc::CipherValue>(builder, intput_index, shape));
  }

  auto output_public(size_t output_index) {
    return executor.output(output_index, 0);
  }

  auto output_arith(size_t output_index) {
    auto x0 = executor.output(output_index, 0);
    auto x1 = executor.output(output_index, 1);
    auto x2 = executor.output(output_index, 2);
    return eager::add(x0, eager::add(x1, x2));
  }

  auto output_boolen(size_t output_index) {
    auto x0 = executor.output(output_index, 0);
    auto x1 = executor.output(output_index, 1);
    auto x2 = executor.output(output_index, 2);
    return eager::_xor(x0, eager::_xor(x1, x2));
  }

protected:
  FluxContext context;
  FluxBuilder builder;
  FluxExecutor executor;
};

namespace {

template <size_t N> auto make_tensor(const uint64_t (&array)[N]) {
  auto tensor = eager::Tensor::with_shape({N});
  std::copy(array, array + N, tensor.data());
  return tensor;
}

template <size_t N, size_t M> auto make_tensor(const uint64_t (&array)[N][M]) {
  auto tensor = eager::Tensor::with_shape({N, M});
  const uint64_t *begin = std::begin(array[0]);
  const uint64_t *end = std::end(array[N - 1]);
  assert(end - begin == N * M);
  std::copy(begin, end, tensor.data());
  return tensor;
}

} // namespace

TEST_F(aby3FunctionTest, test_add_aa) {
  auto x = input_secret(0, make_tensor({114, 514}));
  auto y = input_secret(1, make_tensor({1919, 810}));
  auto result = add_aa(builder, x, y);
  output(builder, 0, cast(result));
  executor.run();
  {
    auto result = output_arith(0);
    EXPECT_EQ(result.at({0}), 114 + 1919);
    EXPECT_EQ(result.at({1}), 514 + 810);
  }
}

TEST_F(aby3FunctionTest, test_multiply_aa) {
  auto x = input_secret(0, make_tensor({114, 514}));
  auto y = input_secret(1, make_tensor({1919, 810}));
  auto result = multiply_aa(builder, x, y);
  output(builder, 0, cast(result));
  executor.run();
  {
    auto result = output_arith(0);
    EXPECT_EQ(result.at({0}), 114 * 1919);
    EXPECT_EQ(result.at({1}), 514 * 810);
  }
}

TEST_F(aby3FunctionTest, test_matmul_aa) {
  auto x = input_secret(0, make_tensor<2, 1>({{114}, {514}}));
  auto y = input_secret(1, make_tensor<1, 2>({{1919, 810}}));
  auto result = matmul_aa(builder, x, y);
  output(builder, 0, cast(result));
  executor.run();
  {
    auto result = output_arith(0);
    EXPECT_EQ(result.at({0, 0}), 218766);
    EXPECT_EQ(result.at({0, 1}), 92340);
    EXPECT_EQ(result.at({1, 0}), 986366);
    EXPECT_EQ(result.at({1, 1}), 416340);
  }
}

TEST_F(aby3FunctionTest, test_xor_bb) {
  auto x = input_secret(0, make_tensor({114, 514}));
  auto y = input_secret(1, make_tensor({1919, 810}));
  auto result = xor_bb(builder, x, y);
  output(builder, 0, cast(result));
  executor.run();
  {
    auto result = output_boolen(0);
    EXPECT_EQ(result.at({0}), 114 ^ 1919);
    EXPECT_EQ(result.at({1}), 514 ^ 810);
  }
}

TEST_F(aby3FunctionTest, test_and_bb) {
  auto x = input_secret(0, make_tensor({114, 514}));
  auto y = input_secret(1, make_tensor({1919, 810}));
  auto result = and_bb(builder, x, y);
  output(builder, 0, cast(result));
  executor.run();
  {
    auto result = output_boolen(0);
    EXPECT_EQ(result.at({0}), 114 & 1919);
    EXPECT_EQ(result.at({1}), 514 & 810);
  }
}

TEST_F(aby3FunctionTest, test_a2b) {
  const int N = 10000;
  auto input = eager::Tensor::with_shape({N});
  std::iota(input.data(), input.data() + N, 0);

  auto x = input_secret(0, input);
  auto result = a2b(builder, x);
  output(builder, 0, cast(result));
  executor.run();
  {
    auto result = output_boolen(0);
    for (size_t i = 0; i < N; i++) {
      EXPECT_EQ(result.at({i}), i);
    }
  }
}

TEST_F(aby3FunctionTest, test_b2a) {
  const int N = 10000;
  auto input = eager::Tensor::with_shape({N});
  std::iota(input.data(), input.data() + N, 0);

  auto x = input_secret(0, input);
  auto result = b2a(builder, x);
  output(builder, 0, cast(result));
  executor.run();
  {
    auto result = output_arith(0);
    for (size_t i = 0; i < N; i++) {
      EXPECT_EQ(result.at({i}), i);
    }
  }
}

} // namespace fastmpc::flux::aby3::testing