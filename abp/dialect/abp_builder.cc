#include "fastmpc/abp/dialect/abp_builder.h"
#include "fastmpc/abp/dialect/abp_context.h"
#include "fastmpc/abp/dialect/abp_ops.h"
#include "fastmpc/abp/dialect/abp_types.h"

#include <cassert>
#include <cstddef>
#include <functional>
#include <numeric>
#include <utility>
#include <vector>

namespace fastmpc::abp {

template <class T> auto ABPBuilder::push_op(T &&op) -> OpHandle {
  return push(Op{
      .kind = T::kind,
      .offset = push(std::move(op)),
  });
}

template <> auto ABPBuilder::push<Op>(Op &&op) -> OpHandle {
  return inner_->ops_.push(std::move(op));
}

template <> auto ABPBuilder::push<InputOp>(InputOp &&op) -> size_t {
  size_t result = inner_->input_ops_.size();
  inner_->input_ops_.push_back(std::move(op));
  return result;
}

auto ABPBuilder::input(size_t input_index, Type type) -> OpHandle {
  return push_op(InputOp{
      .type = type,
      .input_index = input_index,
  });
}

template <> auto ABPBuilder::push<OutputOp>(OutputOp &&op) -> size_t {
  size_t result = inner_->output_ops_.size();
  inner_->output_ops_.push_back(std::move(op));
  return result;
}

auto ABPBuilder::output(OpHandle operand, size_t output_index) -> OpHandle {
  return push_op(OutputOp{
      .operand = operand,
      .output_index = output_index,
  });
}

#define DECL_PUSH(TypeName, ListName)                                          \
  template <>                                                                  \
  auto ABPBuilder::push<TypeName>(TypeName && op) ->                           \
      typename TypeName::handle_type {                                         \
    return inner_->ListName.push(std::move(op));                               \
  }
DECL_PUSH(ConstantOp, constant_ops_)
DECL_PUSH(NegateAOp, neagte_a_ops_)
DECL_PUSH(NegatePOp, neagte_p_ops_)
DECL_PUSH(InverseOp, inverse_ops_)
DECL_PUSH(NotBOp, not_b_ops_)
DECL_PUSH(BitReverseOp, bit_reverse_ops_)
DECL_PUSH(ShiftRightOp, shift_right_ops_)
DECL_PUSH(P2AOp, p2a_ops_)
DECL_PUSH(A2BOp, a2b_ops_)
DECL_PUSH(B2AOp, b2a_ops_)
DECL_PUSH(BroadcastOp, broadcast_ops_)
DECL_PUSH(ReshapeOp, reshape_ops_)
DECL_PUSH(SliceOp, slice_ops_)
DECL_PUSH(TransposeOp, transpose_ops_)
DECL_PUSH(AddAAOp, add_aa_ops_)
DECL_PUSH(AddAPOp, add_ap_ops_)
DECL_PUSH(AddPPOp, add_pp_ops_)
DECL_PUSH(MultiplyAAOp, multiply_aa_ops_)
DECL_PUSH(MultiplyAPOp, multiply_ap_ops_)
DECL_PUSH(MultiplyPPOp, multiply_pp_ops_)
DECL_PUSH(XorBBOp, xor_bb_ops_)
DECL_PUSH(AndBBOp, and_bb_ops_)
DECL_PUSH(DotGeneralAAOp, dot_general_ops_)
DECL_PUSH(ConcateOp, concat_ops_)
DECL_PUSH(TruncateAOp, truncate_a_ops_)
DECL_PUSH(TruncatePOp, truncate_p_ops_)
#undef DECL_PUSH

#define DECL_UNARY_OP(TypeName, FuncName, t)                                   \
  auto ABPBuilder::FuncName(OpHandle operand) -> OpHandle {                    \
    assert(is_##t(operand));                                                   \
    return push_op(TypeName{                                                   \
        .type = inner_->type(operand),                                         \
        .operand = operand,                                                    \
    });                                                                        \
  }
DECL_UNARY_OP(NegateAOp, negate_a, a)
DECL_UNARY_OP(NegatePOp, negate_p, p)
DECL_UNARY_OP(InverseOp, inverse, p)
DECL_UNARY_OP(NotBOp, not_b, b)
DECL_UNARY_OP(BitReverseOp, bit_reverse, b)
#undef DECL_UNARY_OP

#define DECL_ADD_OP(TypeName, FuncName, t)                                     \
  auto ABPBuilder::FuncName(OpHandle left, OpHandle right) -> OpHandle {       \
    assert(is_##t(left, right) && check_shape(left, right) &&                  \
           check_fixed_point(left, right));                                    \
    return push_op(TypeName{                                                   \
        .type = inner_->type(left),                                            \
        .left = left,                                                          \
        .right = right,                                                        \
    });                                                                        \
  }

DECL_ADD_OP(AddAAOp, add_aa, aa)
DECL_ADD_OP(AddAPOp, add_ap, ap)
DECL_ADD_OP(AddPPOp, add_pp, pp)
#undef DECL_ADD_OP

#define DECL_MUL_OP(TypeName, FuncName, t)                                     \
  auto ABPBuilder::FuncName(OpHandle left, OpHandle right) -> OpHandle {       \
    assert(is_##t(left, right) && check_shape(left, right));                   \
    auto type = inner_->type(left);                                            \
    auto right_point = inner_->type(right).fixed_point;                        \
    type.fixed_point += right_point;                                           \
    return push_op(TypeName{                                                   \
        .type = type,                                                          \
        .left = left,                                                          \
        .right = right,                                                        \
    });                                                                        \
  }

DECL_MUL_OP(MultiplyAAOp, multiply_aa, aa)
DECL_MUL_OP(MultiplyAPOp, multiply_ap, ap)
DECL_MUL_OP(MultiplyPPOp, multiply_pp, pp)
#undef DECL_MUL_OP

#define DECL_BIT_OP(TypeName, FuncName, t)                                     \
  auto ABPBuilder::FuncName(OpHandle left, OpHandle right) -> OpHandle {       \
    assert(is_##t(left, right) && check_shape(left, right));                   \
    return push_op(TypeName{                                                   \
        .type = inner_->type(left),                                            \
        .left = left,                                                          \
        .right = right,                                                        \
    });                                                                        \
  }

DECL_BIT_OP(AndBBOp, and_bb, bb)
DECL_BIT_OP(XorBBOp, xor_bb, bb)
#undef DECL_BIT_OP

auto ABPBuilder::dot_general_aa(OpHandle left, OpHandle right) -> OpHandle {
  assert(is_aa(left, right));
  auto left_type = inner_->type(left);
  auto right_type = inner_->type(right);
  auto &left_shape = inner_->shape(left);
  auto &right_shape = inner_->shape(right);
  assert(left_shape.size() == 2 && right_shape.size() == 2);
  assert(left_shape[1] == right_shape[0]);
  uint8_t fixed_point = left_type.fixed_point + right_type.fixed_point;
  auto type = Type{
      .kind = left_type.kind,
      .fixed_point = fixed_point,
      .shape = push(Shape{left_shape[0], right_shape[1]}),
  };
  return push_op(DotGeneralAAOp{
      .type = type,
      .left = left,
      .right = right,
  });
}

auto ABPBuilder::divide_pow_of_2(OpHandle operand, uint8_t pow) -> OpHandle {
  assert(is_a(operand));
  if (pow == 0) {
    return operand;
  } else {
    assert(pow > 0);
    return push_op(TruncateAOp{
        .type = inner_->type(operand),
        .operand = operand,
        .bits = pow,
    });
  }
}

auto ABPBuilder::truncate_a(OpHandle operand, uint8_t bits) -> OpHandle {
  assert(is_a(operand) && bits > 0);
  auto type = inner_->type(operand);
  type.fixed_point -= bits;
  return push_op(TruncateAOp{
      .type = type,
      .operand = operand,
      .bits = bits,
  });
}

auto ABPBuilder::truncate_p(OpHandle operand, uint8_t bits) -> OpHandle {
  assert(is_p(operand) && bits > 0);
  auto type = inner_->type(operand);
  type.fixed_point -= bits;
  return push_op(TruncatePOp{
      .type = type,
      .operand = operand,
      .bits = bits,
  });
}

auto ABPBuilder::shift_right(OpHandle operand, uint8_t bits) -> OpHandle {
  assert(is_b(operand));
  if (bits == 0) {
    return operand;
  }
  assert(bits > 0);
  return push_op(ShiftRightOp{
      .type = inner_->type(operand),
      .operand = operand,
      .bits = bits,
  });
}

auto ABPBuilder::p2a(OpHandle operand) -> OpHandle {
  assert(is_p(operand));
  auto type = inner_->type(operand);
  type.kind = TypeKind::kArithFixed64;
  return push_op(P2AOp{
      .type = type,
      .operand = operand,
  });
}

auto ABPBuilder::a2b(OpHandle operand) -> OpHandle {
  assert(is_a(operand));
  auto type = inner_->type(operand);
  type.kind = TypeKind::kBitArray64;
  type.fixed_point = 0;
  return push_op(A2BOp{
      .type = type,
      .operand = operand,
  });
}

auto ABPBuilder::b2a(OpHandle operand, uint8_t fixed_point) -> OpHandle {
  assert(is_b(operand));
  auto type = inner_->type(operand);
  type.kind = TypeKind::kArithFixed64;
  type.fixed_point = fixed_point;
  return push_op(B2AOp{
      .type = type,
      .operand = operand,
  });
}

auto ABPBuilder::constant(DenseValueHandle value, Type type) -> OpHandle {
  // TODO: check shape
  return push_op(ConstantOp{
      .type = type,
      .value = value,
  });
}

namespace {

[[maybe_unused]] auto check_broadcast(const Shape &input, const Shape &output,
                                      const DenseSizeT &dimensions) -> bool {
  if (input.size() != dimensions.size())
    return false;
  for (size_t i = 0; i < dimensions.size(); i++) {
    if (output[dimensions[i]] != input[i] && input[i] != 1)
      return false;
  }
  return true;
}

} // namespace

auto ABPBuilder::broadcast(OpHandle operand, DenseSizeT dimensions,
                           ShapeHandle shape) -> OpHandle {
  assert(check_broadcast(inner_->shape(operand), inner_->shape(shape),
                         dimensions));
  auto type = inner_->type(operand);
  type.shape = shape;
  return push_op(BroadcastOp{
      .type = type,
      .operand = operand,
      .dimensions = push(std::move(dimensions)),
  });
}

auto ABPBuilder::concate(std::vector<OpHandle> &&operands,
                         size_t dimension) -> OpHandle {
  auto base_type = inner_->type(operands[0]);
  auto new_shape = inner_->shape(operands[0]);
  for (size_t i = 1; i < operands.size(); i++) {
    auto this_type = inner_->type(operands[i]);
    auto &this_shape = inner_->shape(operands[i]);
    assert(base_type.kind == this_type.kind);
    assert(base_type.fixed_point == this_type.fixed_point);
    assert(this_shape.size() == new_shape.size());
    for (size_t j = 0; j < new_shape.size(); j++) {
      if (j == dimension) {
        new_shape[j] += this_shape[j];
      } else {
        assert(new_shape[j] == this_shape[j]);
      }
    }
  }
  Type new_type{
      .kind = base_type.kind,
      .fixed_point = base_type.fixed_point,
      .shape = push(std::move(new_shape)),
  };
  return push_op(ConcateOp{
      .type = new_type,
      .operands = std::move(operands),
      .dimension = dimension,
  });
}

namespace {

[[maybe_unused]] auto check_reshape(const Shape &shape,
                                    const Shape &new_shape) {
  auto element_size =
      std::accumulate(shape.begin(), shape.end(), 1ul, std::multiplies<>());
  auto new_element_size = std::accumulate(new_shape.begin(), new_shape.end(),
                                          1ul, std::multiplies<>());
  return element_size == new_element_size;
}

} // namespace

auto ABPBuilder::reshape(OpHandle operand, ShapeHandle shape) -> OpHandle {
  assert(check_reshape(inner_->shape(operand), inner_->shape(shape)));
  auto type = inner_->type(operand);
  type.shape = shape;
  return push_op(ReshapeOp{
      .type = type,
      .operand = operand,
  });
}

auto ABPBuilder::transpose(OpHandle operand,
                           DenseSizeT permutation) -> OpHandle {
  auto type = inner_->type(operand);
  auto &in_shape = inner_->shape(operand);
  assert(in_shape.size() == permutation.size());
  Shape out_shape(in_shape.size());
  for (size_t i = 0; i < out_shape.size(); i++) {
    out_shape[permutation[i]] = in_shape[i];
  }
  type.shape = push(std::move(out_shape));
  return push_op(TransposeOp{
      .type = type,
      .operand = operand,
      .permutation = push(std::move(permutation)),
  });
}

namespace {

auto slice_shape(const DenseSizeT &start, const DenseSizeT &end,
                 const DenseSizeT &stride) {
  assert(start.size() == end.size());
  assert(start.size() == start.size());
  Shape shape(start.size());
  for (size_t i = 0; i < shape.size(); i++) {
    shape[i] = (end[i] - start[i] + stride[i] - 1) / stride[i];
  }
  return shape;
}

} // namespace

auto ABPBuilder::slice(OpHandle operand, DenseSizeT start,
                       DenseSizeT end) -> OpHandle {
  return slice(operand, start, end, DenseSizeT(start.size(), 1));
}

auto ABPBuilder::slice(OpHandle operand, DenseSizeT start, DenseSizeT end,
                       DenseSizeT stride) -> OpHandle {
  auto type = inner_->type(operand);
  type.shape = push(slice_shape(start, end, stride));
  return push_op(SliceOp{
      .type = type,
      .operand = operand,
      .start = push(std::move(start)),
      .end = push(std::move(end)),
      .stride = push(std::move(stride)),
  });
}

auto ABPBuilder::is_a(OpHandle operand) const -> bool {
  return inner_->type(operand).kind == TypeKind::kArithFixed64;
}

auto ABPBuilder::is_b(OpHandle operand) const -> bool {
  return inner_->type(operand).kind == TypeKind::kBitArray64;
}

auto ABPBuilder::is_p(OpHandle operand) const -> bool {
  return inner_->type(operand).kind == TypeKind::kFixed64;
}

auto ABPBuilder::is_aa(OpHandle left, OpHandle right) const -> bool {
  return is_a(left) && is_a(right);
}

auto ABPBuilder::is_bb(OpHandle left, OpHandle right) const -> bool {
  return is_b(left) && is_b(right);
}

auto ABPBuilder::is_ap(OpHandle left, OpHandle right) const -> bool {
  return is_a(left) && is_p(right);
}

auto ABPBuilder::is_pp(OpHandle left, OpHandle right) const -> bool {
  return is_p(left) && is_p(right);
}

auto ABPBuilder::check_shape(OpHandle left, OpHandle right) const -> bool {
  auto left_shape = inner_->type(left).shape;
  auto right_shape = inner_->type(right).shape;
  return left_shape == right_shape;
}

auto ABPBuilder::check_fixed_point(OpHandle left,
                                   OpHandle right) const -> bool {
  uint8_t left_point = inner_->type(left).fixed_point;
  uint8_t right_point = inner_->type(right).fixed_point;
  if (left_point != right_point)
    std::abort();
  return left_point == right_point;
}

} // namespace fastmpc::abp