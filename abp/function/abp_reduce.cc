#include "fastmpc/abp/function/abp_reduce.h"
#include "fastmpc/abp/dialect/abp_builder.h"
#include "fastmpc/abp/dialect/abp_ops.h"

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <numeric>
#include <utility>
#include <vector>

namespace fastmpc::abp {

namespace {

auto tree_reduce_power_of_two(ABPBuilder &builder, OpHandle x,
                              ReduceFunc reducer) {
  auto &context = builder.context();
  auto x_type = context.type(x);
  auto x_shape = context.shape(x_type.shape);
  assert(x_shape.size() == 2);
  size_t batch_size = x_shape[0];
  size_t reduce_size = x_shape[1];
  while (reduce_size != 1) {
    size_t povit = reduce_size / 2;
    auto left = builder.slice(x, {0, 0}, {batch_size, povit});
    auto right = builder.slice(x, {0, povit}, {batch_size, reduce_size});
    x = reducer(builder, left, right);
    reduce_size /= 2;
  }
  return x;
}

// x must be a 2d-tensor, first dimension is the batched dimention, second
// dimension is the reduced dimension
auto tree_reduce(ABPBuilder &builder, OpHandle x, ReduceFunc reducer) {
  auto &context = builder.context();
  auto x_type = context.type(x);
  auto x_shape = context.shape(x_type.shape);
  assert(x_shape.size() == 2);
  size_t batch_size = x_shape[0];
  size_t reduce_size = x_shape[1];
  if (__builtin_popcountll(reduce_size) == 1) {
    return tree_reduce_power_of_two(builder, x, reducer);
  }
  size_t n = 1;
  while (n + n < reduce_size) {
    n += n;
  }

  // reduce(x[::, 0:extend], x[::, n::reduce_size])
  size_t extend = reduce_size - n;
  auto left = builder.slice(x, {0, 0}, {batch_size, extend});
  auto right = builder.slice(x, {0, n}, {batch_size, reduce_size});

  left = reducer(builder, left, right);
  right = builder.slice(x, {0, extend}, {batch_size, n});
  auto result = builder.concate({left, right}, 1);
  return tree_reduce_power_of_two(builder, result, reducer);
}

} // namespace

auto reduce(ABPBuilder &builder, OpHandle x, OpHandle init, DenseSizeT &&dims,
            Shape &&shape, ReduceFunc reducer) -> OpHandle {
  auto &context = builder.context();
  auto x_type = context.type(x);
  auto x_shape = context.shape(x_type.shape);

  auto is_reduced_dim = [&dims](size_t i) -> bool {
    return std::find(dims.begin(), dims.end(), i) != dims.end();
  };

  size_t batched_idx = 0;
  size_t reduced_idx = x_shape.size() - dims.size();

  size_t batched_size = 1;
  size_t reduced_size = 1;

  Shape new_shape(x_shape.size());
  DenseSizeT inverse(x_shape.size());
  DenseSizeT permutation(x_shape.size());
  for (size_t i = 0; i < x_shape.size(); i++) {
    if (is_reduced_dim(i)) {
      permutation[i] = reduced_idx++;
      reduced_size *= x_shape[i];
    } else {
      permutation[i] = batched_idx++;
      batched_size *= x_shape[i];
    }
    new_shape[permutation[i]] = x_shape[i];
    inverse[permutation[i]] = i;
  }
  assert(batched_idx == x_shape.size() - dims.size());
  assert(reduced_idx == x_shape.size());

  bool need_transpose = false;
  for (size_t i = 0; i < x_shape.size(); i++) {
    if (permutation[i] != i) {
      need_transpose = true;
      break;
    }
  }
  if (need_transpose) {
    x = builder.transpose(x, std::move(permutation));
  }
  bool need_reshape = false;
  Shape std_reduce_shape = {batched_size, reduced_size};
  if (std_reduce_shape != new_shape) {
    need_reshape = true;
    auto handle = builder.push(std::move(std_reduce_shape));
    x = builder.reshape(x, handle);
  }
  x = tree_reduce(builder, x, reducer);
  init = builder.broadcast(init, {}, builder.push(Shape{batched_size, 1}));
  x = reducer(builder, x, init);
  if (need_reshape) {
    for (size_t i = 0; i < dims.size(); i++) {
      new_shape[x_shape.size() - dims.size() + i] = 1;
    }
    x = builder.reshape(x, builder.push(std::move(new_shape)));
  }

  if (need_transpose) {
    x = builder.transpose(x, std::move(inverse));
  }
  x_type = context.type(x);
  x_shape = context.shape(x_type.shape);
  if (x_shape != shape) {
    x = builder.reshape(x, builder.push(std::move(shape)));
  }
  return x;
}

} // namespace fastmpc::abp