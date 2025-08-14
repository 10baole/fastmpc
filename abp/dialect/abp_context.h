#pragma once

#include "fastmpc/abp/dialect/abp_ops.h"
#include "fastmpc/abp/dialect/abp_types.h"

#include <iosfwd>
#include <vector>

#include "fastmpc/ir_base/unique_vector.h"

namespace fastmpc::abp {

class ValuePrinter {
public:
  virtual ~ValuePrinter() = default;
  virtual void print_value(std::ostream &out, OpHandle handle) = 0;
};

class ABPContext {
public:
  ABPContext() = default;
  auto type(OpHandle op) const -> Type;
  auto dense_value(DenseValueHandle handle) const -> const DenseValue & {
    return dense_value_list_[handle.unwarp()];
  }

  void print(std::ostream &out, ValuePrinter *printer = nullptr) const;
  void print(std::ostream &out, OpHandle op) const;

  auto shape(OpHandle op) const -> const Shape & {
    return shape(type(op).shape);
  }

  auto dense_size_t(DenseSizeTHandle handle) const -> const DenseSizeT & {
    return dense_size_t_list_[handle.unwarp()];
  }

  auto shape(ShapeHandle handle) const -> const Shape & {
    return shape_list_[handle.unwarp()];
  }

  auto ops_size() const -> size_t { return ops_.size(); }

  template <class Func> auto visit(OpHandle handle, Func &&func) const {
    auto op = ops_[handle.unwarp()];
    switch (op.kind) {
    case OpKind::kInputOp:
      return func(handle, input_ops_[op.offset]);
    case OpKind::kOutputOp:
      return func(handle, output_ops_[op.offset]);
    case OpKind::kConstantOp:
      return func(handle, constant_ops_[op.offset]);
    case OpKind::kNegateAOp:
      return func(handle, neagte_a_ops_[op.offset]);
    case OpKind::kNegatePOp:
      return func(handle, neagte_p_ops_[op.offset]);
    case OpKind::kInverseOp:
      return func(handle, inverse_ops_[op.offset]);
    case OpKind::kTruncateAOp:
      return func(handle, truncate_a_ops_[op.offset]);
    case OpKind::kTruncatePOp:
      return func(handle, truncate_p_ops_[op.offset]);
    case OpKind::kNotBOp:
      return func(handle, not_b_ops_[op.offset]);
    case OpKind::kBitReverseOp:
      return func(handle, bit_reverse_ops_[op.offset]);
    case OpKind::kShiftRightOp:
      return func(handle, shift_right_ops_[op.offset]);
    case OpKind::kP2AOp:
      return func(handle, p2a_ops_[op.offset]);
    case OpKind::kA2BOp:
      return func(handle, a2b_ops_[op.offset]);
    case OpKind::kB2AOp:
      return func(handle, b2a_ops_[op.offset]);
    case OpKind::kBroadcastOp:
      return func(handle, broadcast_ops_[op.offset]);
    case OpKind::kReshapeOp:
      return func(handle, reshape_ops_[op.offset]);
    case OpKind::kSliceOp:
      return func(handle, slice_ops_[op.offset]);
    case OpKind::kTransposeOp:
      return func(handle, transpose_ops_[op.offset]);
    case OpKind::kAddAAOp:
      return func(handle, add_aa_ops_[op.offset]);
    case OpKind::kAddAPOp:
      return func(handle, add_ap_ops_[op.offset]);
    case OpKind::kAddPPOp:
      return func(handle, add_pp_ops_[op.offset]);
    case OpKind::kMultiplyAAOp:
      return func(handle, multiply_aa_ops_[op.offset]);
    case OpKind::kMultiplyAPOp:
      return func(handle, multiply_ap_ops_[op.offset]);
    case OpKind::kMultiplyPPOp:
      return func(handle, multiply_pp_ops_[op.offset]);
    case OpKind::kXorBBOp:
      return func(handle, xor_bb_ops_[op.offset]);
    case OpKind::kAndBBOp:
      return func(handle, and_bb_ops_[op.offset]);
    case OpKind::kDotGeneralAAOp:
      return func(handle, dot_general_ops_[op.offset]);
    case OpKind::kConcateOp:
      return func(handle, concat_ops_[op.offset]);
    }
  }

private:
  friend class ABPBuilder;

  // types
  UniqueVector<Shape> shape_list_;

  // ops
  UniqueVector<DenseValue> dense_value_list_;
  UniqueVector<DenseSizeT> dense_size_t_list_;
  std::vector<InputOp> input_ops_;
  std::vector<OutputOp> output_ops_;
  UniqueVector<ConstantOp> constant_ops_;

  // ops.unary
  UniqueVector<NegateAOp> neagte_a_ops_;
  UniqueVector<NegatePOp> neagte_p_ops_;
  UniqueVector<InverseOp> inverse_ops_;
  UniqueVector<NotBOp> not_b_ops_;
  UniqueVector<BitReverseOp> bit_reverse_ops_;
  UniqueVector<ShiftRightOp> shift_right_ops_;
  UniqueVector<P2AOp> p2a_ops_;
  UniqueVector<A2BOp> a2b_ops_;
  UniqueVector<B2AOp> b2a_ops_;
  UniqueVector<BroadcastOp> broadcast_ops_;
  UniqueVector<ReshapeOp> reshape_ops_;
  UniqueVector<SliceOp> slice_ops_;
  UniqueVector<TransposeOp> transpose_ops_;
  UniqueVector<TruncateAOp> truncate_a_ops_;
  UniqueVector<TruncatePOp> truncate_p_ops_;

  // ops.binary
  UniqueVector<AddAAOp> add_aa_ops_;
  UniqueVector<AddAPOp> add_ap_ops_;
  UniqueVector<AddPPOp> add_pp_ops_;
  UniqueVector<MultiplyAAOp> multiply_aa_ops_;
  UniqueVector<MultiplyAPOp> multiply_ap_ops_;
  UniqueVector<MultiplyPPOp> multiply_pp_ops_;
  UniqueVector<XorBBOp> xor_bb_ops_;
  UniqueVector<AndBBOp> and_bb_ops_;
  UniqueVector<DotGeneralAAOp> dot_general_ops_;

  UniqueVector<ConcateOp> concat_ops_;

  UniqueVector<Op> ops_;
};

} // namespace fastmpc::abp