#include "fastmpc/flux/executor/flux_executor.h"

#include "fastmpc/eager/tensor.h"
#include "fastmpc/eager/tensor_ops.h"
#include "fastmpc/flux/dialect/flux_context.h"
#include "fastmpc/flux/dialect/flux_ops.h"
#include <algorithm>
#include <cassert>
#include <numeric>

namespace fastmpc::flux {

void FluxExecutor::run() {
  for (size_t i = 0; i < context_->ops_size(); i++) {
    context_->visit(OpHandle(i), *this);
  }
}

void FluxExecutor::print_value(std::ostream &out, OpHandle handle) {
  auto it = map_.find(handle);
  if (it != map_.end()) {
    it->second.print(out);
  }
}

void FluxExecutor::push(OpHandle key, eager::Tensor value) {
  assert(value.data() && value.shape().size() < 10);
  auto [_, success] = map_.emplace(key, value);
  assert(success);
}

auto FluxExecutor::get(OpHandle key) -> eager::Tensor {
  auto it = map_.find(key);
  assert(it != map_.end() && it->second.shape().size() < 10);
  return it->second;
}

void FluxExecutor::operator()(OpHandle handle, InputOp op) {
  auto it = inputs_.find({op.input_index, op.tuple_index});
  assert(it != inputs_.end());
  push(handle, it->second);
}

void FluxExecutor::operator()(OpHandle handle, OutputOp op) {
  auto value = get(op.operand);
  auto key = std::make_pair(op.output_index, op.tuple_index);
  auto [iter, success] = outputs_.emplace(key, value);
  if (!success) {
    assert(eager::equal(iter->second, value));
  }
}

void FluxExecutor::operator()(OpHandle handle, AShiftRightOp op) {
  auto operand = get(op.operand);
  push(handle, eager::arith_shift_right(operand, op.bits));
}

void FluxExecutor::operator()(OpHandle handle, BitReverseOp op) {
  auto operand = get(op.operand);
  push(handle, eager::bit_reverse(operand));
}

void FluxExecutor::operator()(OpHandle handle, BroadcastOp op) {
  auto operand = get(op.operand);
  auto &shape = context_->shape(op.type.shape);
  auto &dimensions = context_->dense_size_t(op.dimensions);
  push(handle, operand.broadcast(shape, dimensions));
}

void FluxExecutor::operator()(OpHandle handle, CastOp op) {
  auto result = get(op.operand);
  push(handle, result);
}

void FluxExecutor::operator()(OpHandle handle, InverseOp op) {
  int64_t scalar = 1ll << op.fixed_point;
  auto operand = get(op.operand);
  push(handle, eager::inverse(operand, scalar));
}

void FluxExecutor::operator()(OpHandle handle, LShiftRightOp op) {
  auto operand = get(op.operand);
  push(handle, eager::logic_shift_right(operand, op.bits));
}

void FluxExecutor::operator()(OpHandle handle, NegateOp op) {
  auto operand = get(op.operand);
  push(handle, eager::nagete(operand));
}

void FluxExecutor::operator()(OpHandle handle, NotOp op) {
  auto operand = get(op.operand);
  push(handle, eager::_not(operand));
}

void FluxExecutor::operator()(OpHandle handle, ReshapeOp op) {
  auto operand = get(op.operand);
  auto &shape = context_->shape(op.type.shape);
  push(handle, operand.reshape(shape));
}

void FluxExecutor::operator()(OpHandle handle, ShiftLeftOp op) {
  auto operand = get(op.operand);
  push(handle, eager::shift_left(operand, op.bits));
}

void FluxExecutor::operator()(OpHandle handle, SliceOp op) {
  auto operand = get(op.operand);
  auto &start = context_->dense_size_t(op.start);
  auto &end = context_->dense_size_t(op.end);
  auto &stride = context_->dense_size_t(op.stride);

  // hack: Stride of SliceOp must be unsigned integer.
  auto signed_data = reinterpret_cast<const int64_t *>(stride.data());
  absl::Span<const int64_t> hacked(signed_data, stride.size());
  push(handle, operand.slice(start, end, hacked));
}

void FluxExecutor::operator()(OpHandle handle, TransposeOp op) {
  auto operand = get(op.operand);
  auto &permutation = context_->dense_size_t(op.permutation);
  push(handle, operand.transpose(permutation));
}

void FluxExecutor::operator()(OpHandle handle, AddOp op) {
  auto x = get(op.left);
  auto y = get(op.right);
  push(handle, eager::add(x, y));
}

void FluxExecutor::operator()(OpHandle handle, AndOp op) {
  auto x = get(op.left);
  auto y = get(op.right);
  push(handle, eager::_and(x, y));
}

void FluxExecutor::operator()(OpHandle handle, MatmulOp op) {
  auto x = get(op.left);
  auto y = get(op.right);
  push(handle, eager::matmul(x, y));
}

void FluxExecutor::operator()(OpHandle handle, MultiplyOp op) {
  auto x = get(op.left);
  auto y = get(op.right);
  push(handle, eager::multiply(x, y));
}

void FluxExecutor::operator()(OpHandle handle, SubtractOp op) {
  auto x = get(op.left);
  auto y = get(op.right);
  push(handle, eager::subtract(x, y));
}

void FluxExecutor::operator()(OpHandle handle, XorOp op) {
  auto x = get(op.left);
  auto y = get(op.right);
  push(handle, eager::_xor(x, y));
}

void FluxExecutor::operator()(OpHandle handle, ConstantOp op) {
  auto result = eager::Tensor(context_->shape(op.type.shape));
  auto &value = context_->dense_value(op.value).as_vector();
  std::copy(value.begin(), value.end(), result.data());
  push(handle, result);
}

void FluxExecutor::operator()(OpHandle handle, RandomOp op) {
  auto result = eager::Tensor(context_->shape(op.type.shape));
  std::iota(result.data(), result.data() + result.num_elements(), op.rng_seed);
  push(handle, result);
}

void FluxExecutor::operator()(OpHandle handle, ConcateOp op) {
  eager::InlinedVector<eager::Tensor> operands;
  operands.reserve(op.operands.size());
  for (auto operand : op.operands) {
    operands.push_back(get(operand));
  }
  push(handle, eager::Tensor::concate(operands, op.dimension));
}

} // namespace fastmpc::flux