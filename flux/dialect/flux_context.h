#pragma once

#include "fastmpc/flux/dialect/flux_ops.h"

#include <vector>

#include "fastmpc/ir_base/attribute.h"
#include "fastmpc/ir_base/unique_vector.h"

namespace fastmpc::flux {

class ValuePrinter {
public:
  virtual ~ValuePrinter() = default;
  virtual void print_value(std::ostream &out, OpHandle handle) = 0;
};

class FluxContext {
public:
  FluxContext() = default;
  void print(std::ostream &out, ValuePrinter *printer = nullptr) const;
  auto ops_size() const -> size_t { return ops_.size(); }
  auto shape(OpHandle op) const -> const Shape & {
    return shape(type(op).shape);
  }
  auto shape(ShapeHandle handle) const -> const Shape & {
    return shape_list_[handle.unwarp()];
  }
  auto type(OpHandle op) const -> Type;
  auto dense_value(DenseValueHandle handle) const -> const DenseValue & {
    return dense_value_list_[handle.unwarp()];
  }
  auto dense_size_t(DenseSizeTHandle handle) const -> const DenseSizeT & {
    return dense_size_t_list_[handle.unwarp()];
  }

  template <class Func> auto visit(OpHandle handle, Func &&func) const {
    auto op = ops_[handle.unwarp()];
    switch (op.kind) {
    case OpKind::kInputOp:
      return func(handle, input_ops_[op.offset]);
    case OpKind::kOutputOp:
      return func(handle, output_ops_[op.offset]);
    case OpKind::kAShiftRightOp:
      return func(handle, a_shift_right_ops_[op.offset]);
    case OpKind::kBitReverseOp:
      return func(handle, bit_reverse_ops_[op.offset]);
    case OpKind::kBroadcastOp:
      return func(handle, broadcast_ops_[op.offset]);
    case OpKind::kCastOp:
      return func(handle, cast_ops_[op.offset]);
    case OpKind::kInverseOp:
      return func(handle, inverse_ops_[op.offset]);
    case OpKind::kLShiftRightOp:
      return func(handle, l_shift_right_ops_[op.offset]);
    case OpKind::kNegateOp:
      return func(handle, negate_ops_[op.offset]);
    case OpKind::kNotOp:
      return func(handle, not_ops_[op.offset]);
    case OpKind::kReshapeOp:
      return func(handle, reshape_ops_[op.offset]);
    case OpKind::kShiftLeftOp:
      return func(handle, shift_left_ops_[op.offset]);
    case OpKind::kSliceOp:
      return func(handle, slice_ops_[op.offset]);
    case OpKind::kTransposeOp:
      return func(handle, transpose_ops_[op.offset]);
    case OpKind::kAddOp:
      return func(handle, add_ops_[op.offset]);
    case OpKind::kAndOp:
      return func(handle, and_ops_[op.offset]);
    case OpKind::kMatmulOp:
      return func(handle, matmul_ops_[op.offset]);
    case OpKind::kMultiplyOp:
      return func(handle, multiply_ops_[op.offset]);
    case OpKind::kSubtractOp:
      return func(handle, subtract_ops_[op.offset]);
    case OpKind::kXorOp:
      return func(handle, xor_ops_[op.offset]);
    case OpKind::kConstantOp:
      return func(handle, constant_ops_[op.offset]);
    case OpKind::kRandomOp:
      return func(handle, random_ops_[op.offset]);
    case OpKind::kConcateOp:
      return func(handle, concate_ops_[op.offset]);
    }
  }

private:
  void print(std::ostream &out, OpHandle handle) const;

  friend class FluxBuilder;

  // types
  UniqueVector<Shape> shape_list_;
  UniqueVector<DenseSizeT> dense_size_t_list_;
  UniqueVector<DenseValue> dense_value_list_;

  // ops
  std::vector<InputOp> input_ops_;
  std::vector<OutputOp> output_ops_;
  std::vector<AShiftRightOp> a_shift_right_ops_;
  std::vector<BroadcastOp> broadcast_ops_;
  std::vector<CastOp> cast_ops_;
  std::vector<InverseOp> inverse_ops_;
  std::vector<LShiftRightOp> l_shift_right_ops_;
  std::vector<NegateOp> negate_ops_;
  std::vector<NotOp> not_ops_;
  std::vector<ReshapeOp> reshape_ops_;
  std::vector<ShiftLeftOp> shift_left_ops_;
  std::vector<SliceOp> slice_ops_;
  std::vector<TransposeOp> transpose_ops_;
  std::vector<AddOp> add_ops_;
  std::vector<AndOp> and_ops_;
  std::vector<BitReverseOp> bit_reverse_ops_;
  std::vector<MatmulOp> matmul_ops_;
  std::vector<MultiplyOp> multiply_ops_;
  std::vector<SubtractOp> subtract_ops_;
  std::vector<XorOp> xor_ops_;
  std::vector<ConstantOp> constant_ops_;
  std::vector<RandomOp> random_ops_;
  std::vector<ConcateOp> concate_ops_;
  std::vector<Op> ops_;
};

} // namespace fastmpc::flux