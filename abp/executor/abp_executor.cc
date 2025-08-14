#include "fastmpc/abp/executor/abp_executor.h"

#include <algorithm>
#include <bitset>
#include <cstdint>
#include <ostream>
#include <vector>

#include "fastmpc/abp/dialect/abp_ops.h"
#include "fastmpc/abp/dialect/abp_types.h"
#include "fastmpc/eager/tensor.h"
#include "fastmpc/eager/tensor_ops.h"

#include "absl/container/inlined_vector.h"
#include "absl/types/span.h"

namespace fastmpc::abp {

void ABPExecutor::run() {
  for (size_t i = 0; i < context_->ops_size(); i++) {
    context_->visit(OpHandle(i), *this);
  }
}

void ABPExecutor::operator()(OpHandle handle, InputOp op) {
  map_[handle] = intputs_[op.input_index];
}

void ABPExecutor::operator()(OpHandle handle, OutputOp op) {
  outputs_[op.output_index] = map_[op.operand];
}

void ABPExecutor::operator()(OpHandle handle, ConstantOp op) {
  auto result = eager::Tensor(context_->shape(op.type.shape));
  auto &value = context_->dense_value(op.value).as_vector();
  std::copy(value.begin(), value.end(), result.data());
  map_.emplace(handle, result);
}

void ABPExecutor::operator()(OpHandle handle, NegateAOp op) {
  auto operand = map_.find(op.operand)->second;
  map_.emplace(handle, eager::nagete(operand));
}

void ABPExecutor::operator()(OpHandle handle, NegatePOp op) {
  auto operand = map_.find(op.operand)->second;
  map_.emplace(handle, eager::nagete(operand));
}

void ABPExecutor::operator()(OpHandle handle, InverseOp op) {
  int64_t scalar = 1ll << op.type.fixed_point;
  auto operand = map_.find(op.operand)->second;
  map_.emplace(handle, eager::inverse(operand, scalar));
}

void ABPExecutor::operator()(OpHandle handle, TruncateAOp op) {
  auto operand = map_.find(op.operand)->second;
  map_.emplace(handle, eager::arith_shift_right(operand, op.bits));
}

void ABPExecutor::operator()(OpHandle handle, TruncatePOp op) {
  auto operand = map_.find(op.operand)->second;
  map_.emplace(handle, eager::arith_shift_right(operand, op.bits));
}

void ABPExecutor::operator()(OpHandle handle, NotBOp op) {
  auto operand = map_.find(op.operand)->second;
  map_.emplace(handle, eager::_not(operand));
}

void ABPExecutor::operator()(OpHandle handle, BitReverseOp op) {
  auto operand = map_.find(op.operand)->second;
  map_.emplace(handle, eager::bit_reverse(operand));
}

void ABPExecutor::operator()(OpHandle handle, ShiftRightOp op) {
  auto operand = map_.find(op.operand)->second;
  map_.emplace(handle, eager::logic_shift_right(operand, op.bits));
}

void ABPExecutor::operator()(OpHandle handle, P2AOp op) {
  auto operand = map_.find(op.operand)->second;
  map_.emplace(handle, operand);
}

void ABPExecutor::operator()(OpHandle handle, A2BOp op) {
  auto operand = map_.find(op.operand)->second;
  map_.emplace(handle, operand);
}

void ABPExecutor::operator()(OpHandle handle, B2AOp op) {
  auto operand = map_.find(op.operand)->second;
  map_.emplace(handle, operand);
}

void ABPExecutor::operator()(OpHandle handle, BroadcastOp op) {
  auto operand = map_.find(op.operand)->second;
  auto &shape = context_->shape(op.type.shape);
  auto &dimensions = context_->dense_size_t(op.dimensions);
  map_.emplace(handle, operand.broadcast(shape, dimensions));
}

void ABPExecutor::operator()(OpHandle handle, ReshapeOp op) {
  auto operand = map_.find(op.operand)->second;
  auto &shape = context_->shape(op.type.shape);
  map_.emplace(handle, operand.reshape(shape));
}

void ABPExecutor::operator()(OpHandle handle, SliceOp op) {
  auto operand = map_.find(op.operand)->second;
  auto &start = context_->dense_size_t(op.start);
  auto &end = context_->dense_size_t(op.end);
  auto &stride = context_->dense_size_t(op.stride);

  // hack: Stride of SliceOp must be unsigned integer.
  auto signed_data = reinterpret_cast<const int64_t *>(stride.data());
  absl::Span<const int64_t> hacked(signed_data, stride.size());
  map_.emplace(handle, operand.slice(start, end, hacked));
}

void ABPExecutor::operator()(OpHandle handle, TransposeOp op) {
  auto operand = map_.find(op.operand)->second;
  auto &permutation = context_->dense_size_t(op.permutation);
  map_.emplace(handle, operand.transpose(permutation));
}

void ABPExecutor::operator()(OpHandle handle, AddAAOp op) {
  auto x = map_.find(op.left)->second;
  auto y = map_.find(op.right)->second;
  map_.emplace(handle, eager::add(x, y));
}

void ABPExecutor::operator()(OpHandle handle, AddAPOp op) {
  auto x = map_.find(op.left)->second;
  auto y = map_.find(op.right)->second;
  map_.emplace(handle, eager::add(x, y));
}

void ABPExecutor::operator()(OpHandle handle, AddPPOp op) {
  auto x = map_.find(op.left)->second;
  auto y = map_.find(op.right)->second;
  map_.emplace(handle, eager::add(x, y));
}

void ABPExecutor::operator()(OpHandle handle, MultiplyAAOp op) {
  auto x = map_.find(op.left)->second;
  auto y = map_.find(op.right)->second;
  map_.emplace(handle, eager::multiply(x, y));
}

void ABPExecutor::operator()(OpHandle handle, MultiplyAPOp op) {
  auto x = map_.find(op.left)->second;
  auto y = map_.find(op.right)->second;
  map_.emplace(handle, eager::multiply(x, y));
}

void ABPExecutor::operator()(OpHandle handle, MultiplyPPOp op) {
  auto x = map_.find(op.left)->second;
  auto y = map_.find(op.right)->second;
  map_.emplace(handle, eager::multiply(x, y));
}

void ABPExecutor::operator()(OpHandle handle, XorBBOp op) {
  auto x = map_.find(op.left)->second;
  auto y = map_.find(op.right)->second;
  map_.emplace(handle, eager::_xor(x, y));
}

void ABPExecutor::operator()(OpHandle handle, AndBBOp op) {
  auto x = map_.find(op.left)->second;
  auto y = map_.find(op.right)->second;
  map_.emplace(handle, eager::_and(x, y));
}

void ABPExecutor::operator()(OpHandle handle, DotGeneralAAOp op) {
  auto x = map_.find(op.left)->second;
  auto y = map_.find(op.right)->second;
  map_.emplace(handle, eager::matmul(x, y));
}

void ABPExecutor::operator()(OpHandle handle, ConcateOp op) {
  eager::InlinedVector<eager::Tensor> operands;
  operands.reserve(op.operands.size());
  for (auto operand : op.operands) {
    operands.push_back(map_.find(operand)->second);
  }
  map_.emplace(handle, eager::Tensor::concate(operands, op.dimension));
}

void ABPExecutor::print_value(std::ostream &out, OpHandle handle) {
  // skip `OutputOp`
  if (map_.find(handle) == map_.end()) {
    return;
  }
  auto type = context_->type(handle);
  switch (type.kind) {
  case TypeKind::kBitArray64:
    out << std::bitset<64>(*map_[handle].data());
    break;
  case TypeKind::kArithFixed64:
  case TypeKind::kFixed64: {
    map_[handle].print(out);
    // long scalar = 1ll << type.fixed_point;
    // if (scalar == 1) {
    //   out << *map_[handle].data();
    // } else {
    //   auto i_value = static_cast<int64_t>(*map_[handle].data());
    //   auto f_value = static_cast<float>(i_value) / scalar;
    //   out << f_value;
    // }
  }
  }
}

} // namespace fastmpc::abp