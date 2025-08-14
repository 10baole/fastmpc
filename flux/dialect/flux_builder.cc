#include "fastmpc/flux/dialect/flux_builder.h"

#include <cassert>
#include <functional>
#include <numeric>
#include <utility>

#include "fastmpc/flux/dialect/flux_ops.h"
#include "fastmpc/flux/dialect/flux_types.h"
#include "fastmpc/ir_base/attribute.h"

namespace fastmpc::flux {

#define DECL_PUSH(TypeName, ListName)                                          \
  template <>                                                                  \
  auto FluxBuilder::push<TypeName>(TypeName && op)                             \
      ->typename TypeName::handle_type {                                       \
    auto result = inner_->ListName.size();                                     \
    inner_->ListName.push_back(std::move(op));                                 \
    return result;                                                             \
  }
DECL_PUSH(InputOp, input_ops_)
DECL_PUSH(OutputOp, output_ops_)
DECL_PUSH(AShiftRightOp, a_shift_right_ops_)
DECL_PUSH(BroadcastOp, broadcast_ops_)
DECL_PUSH(CastOp, cast_ops_)
DECL_PUSH(InverseOp, inverse_ops_)
DECL_PUSH(LShiftRightOp, l_shift_right_ops_)
DECL_PUSH(NegateOp, negate_ops_)
DECL_PUSH(NotOp, not_ops_)
DECL_PUSH(ReshapeOp, reshape_ops_)
DECL_PUSH(ShiftLeftOp, shift_left_ops_)
DECL_PUSH(SliceOp, slice_ops_)
DECL_PUSH(TransposeOp, transpose_ops_)
DECL_PUSH(AddOp, add_ops_)
DECL_PUSH(AndOp, and_ops_)
DECL_PUSH(BitReverseOp, bit_reverse_ops_)
DECL_PUSH(MatmulOp, matmul_ops_)
DECL_PUSH(MultiplyOp, multiply_ops_)
DECL_PUSH(SubtractOp, subtract_ops_)
DECL_PUSH(XorOp, xor_ops_)
DECL_PUSH(ConstantOp, constant_ops_)
DECL_PUSH(RandomOp, random_ops_)
DECL_PUSH(ConcateOp, concate_ops_)
#undef DECL_PUSH

template <class T> auto FluxBuilder::push_op(T &&op) -> OpHandle {
  OpHandle result(inner_->ops_.size());
  inner_->ops_.push_back({
      .kind = T::kind,
      .offset = push(std::move(op)),
  });
  return result;
}

auto FluxBuilder::input(size_t input_index, size_t tuple_index, Type type)
    -> OpHandle {
  return push_op(InputOp{
      .type = type,
      .input_index = input_index,
      .tuple_index = tuple_index,
  });
}

auto FluxBuilder::output(OpHandle operand, size_t output_index,
                         size_t tuple_index) -> OpHandle {
  return push_op(OutputOp{
      .operand = operand,
      .output_index = output_index,
      .tuple_index = tuple_index,
  });
}

auto FluxBuilder::arith_shift_right(OpHandle opearnd, uint8_t bits)
    -> OpHandle {
  return push_op(AShiftRightOp{
      .type = inner_->type(opearnd),
      .operand = opearnd,
      .bits = bits,
  });
}

auto FluxBuilder::bit_reverse(OpHandle operand) -> OpHandle {
  return push_op(BitReverseOp{
      .type = inner_->type(operand),
      .operand = operand,
  });
}

auto FluxBuilder::broadcast(OpHandle operand, DenseSizeT &&dimensions,
                            ShapeHandle shape) -> OpHandle {
  auto type = inner_->type(operand);
  type.shape = shape;
  return push_op(BroadcastOp{
      .type = type,
      .operand = operand,
      .dimensions = push(~dimensions),
  });
}

auto FluxBuilder::broadcast(OpHandle operand, DenseSizeTHandle dimensions,
                            ShapeHandle shape) -> OpHandle {
  auto type = inner_->type(operand);
  type.shape = shape;
  return push_op(BroadcastOp{
      .type = type,
      .operand = operand,
      .dimensions = dimensions,
  });
}

auto FluxBuilder::cast(OpHandle operand, size_t holder) -> OpHandle {
  auto type = inner_->type(operand);
  assert(type.holder != holder);
  type.holder = holder;
  return push_op(CastOp{
      .type = type,
      .operand = operand,
  });
}

auto FluxBuilder::inverse(OpHandle operand, uint8_t fixed_point) -> OpHandle {
  return push_op(InverseOp{
      .type = inner_->type(operand),
      .operand = operand,
      .fixed_point = fixed_point,
  });
}

auto FluxBuilder::logic_shift_right(OpHandle operand, uint8_t bits)
    -> OpHandle {
  return push_op(LShiftRightOp{
      .type = inner_->type(operand),
      .operand = operand,
      .bits = bits,
  });
}

auto FluxBuilder::negate(OpHandle operand) -> OpHandle {
  return push_op(NegateOp{
      .type = inner_->type(operand),
      .operand = operand,
  });
}

auto FluxBuilder::_not(OpHandle operand) -> OpHandle {
  return push_op(NotOp{
      .type = inner_->type(operand),
      .operand = operand,
  });
}

auto FluxBuilder::reshape(OpHandle operand, ShapeHandle shape) -> OpHandle {
  auto type = inner_->type(operand);
  type.shape = shape;
  return push_op(ReshapeOp{
      .type = type,
      .operand = operand,
  });
}

auto FluxBuilder::shift_left(OpHandle operand, uint8_t bits) -> OpHandle {
  return push_op(ShiftLeftOp{
      .type = inner_->type(operand),
      .operand = operand,
      .bits = bits,
  });
}

auto FluxBuilder::slice(OpHandle operand, DenseSizeTHandle start_handle,
                        DenseSizeTHandle end_handle,
                        DenseSizeTHandle strides_handle) -> OpHandle {
  auto &start = inner_->dense_size_t(start_handle);
  auto &end = inner_->dense_size_t(end_handle);
  auto &strides = inner_->dense_size_t(strides_handle);
  Shape shape(start.size());
  for (size_t i = 0; i < shape.size(); i++) {
    shape[i] = (end[i] - start[i] + strides[i] - 1) / strides[i];
  }
  auto type = inner_->type(operand);
  type.shape = push(~shape);
  return push_op(SliceOp{
      .type = type,
      .operand = operand,
      .start = start_handle,
      .end = end_handle,
      .stride = strides_handle,
  });
}

auto FluxBuilder::transpose(OpHandle operand, DenseSizeTHandle handle)
    -> OpHandle {
  auto &permutation = inner_->dense_size_t(handle);
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
      .permutation = handle,
  });
}

auto FluxBuilder::add(OpHandle left, OpHandle right) -> OpHandle {
  assert(check_holder(left, right) && check_shape(left, right));
  return push_op(AddOp{
      .type = inner_->type(left),
      .left = left,
      .right = right,
  });
}

auto FluxBuilder::_and(OpHandle left, OpHandle right) -> OpHandle {
  assert(check_holder(left, right) && check_shape(left, right));
  return push_op(AndOp{
      .type = inner_->type(left),
      .left = left,
      .right = right,
  });
}

auto FluxBuilder::matmul(OpHandle left, OpHandle right) -> OpHandle {
  assert(check_holder(left, right));
  auto left_shape = inner_->shape(left);
  auto right_shape = inner_->shape(right);
  assert(left_shape.size() == 2 && right_shape.size() == 2);
  assert(left_shape[1] == right_shape[0]);
  auto type = inner_->type(left);
  type.shape = push(Shape{left_shape[0], right_shape[1]});
  return push_op(MatmulOp{
      .type = type,
      .left = left,
      .right = right,
  });
}

auto FluxBuilder::multiply(OpHandle left, OpHandle right) -> OpHandle {
  assert(check_holder(left, right) && check_shape(left, right));
  return push_op(MultiplyOp{
      .type = inner_->type(left),
      .left = left,
      .right = right,
  });
}

auto FluxBuilder::subtract(OpHandle left, OpHandle right) -> OpHandle {
  assert(check_holder(left, right) && check_shape(left, right));
  return push_op(SubtractOp{
      .type = inner_->type(left),
      .left = left,
      .right = right,
  });
}

auto FluxBuilder::_xor(OpHandle left, OpHandle right) -> OpHandle {
  assert(check_holder(left, right) && check_shape(left, right));
  return push_op(XorOp{
      .type = inner_->type(left),
      .left = left,
      .right = right,
  });
}

auto FluxBuilder::constant(DenseValueHandle value, Type type) -> OpHandle {
  return push_op(ConstantOp{
      .type = type,
      .value = value,
  });
}

auto FluxBuilder::constant(DenseValue &&value, Type type) -> OpHandle {
  return push_op(ConstantOp{
      .type = type,
      .value = push(~value),
  });
}

namespace {

auto num_elements(const Shape &shape) {
  return std::accumulate(shape.begin(), shape.end(), 1ul, std::multiplies<>());
}

} // namespace

auto FluxBuilder::random(size_t p0, size_t p1, ShapeHandle shape)
    -> std::pair<OpHandle, OpHandle> {
  if (p0 > p1) {
    auto result = random(p1, p0, shape);
    return {result.second, result.first};
  }
  auto it = rng_seed_.find({p0, p1});
  if (it == rng_seed_.end()) {
    it = rng_seed_.emplace(std::make_pair(p0, p1), 0).first;
  }
  size_t rng_seed = it->second;
  it->second += num_elements(inner_->shape(shape));

  auto first = push_op(RandomOp{
      .type = {p0, shape},
      .rng_index = p1,
      .rng_seed = rng_seed,
  });
  auto second = push_op(RandomOp{
      .type = {p1, shape},
      .rng_index = p0,
      .rng_seed = rng_seed,
  });
  return std::make_pair(first, second);
}

auto FluxBuilder::concate(std::vector<OpHandle> &&operands, size_t dimension)
    -> OpHandle {
  auto base_type = inner_->type(operands[0]);
  auto new_shape = inner_->shape(operands[0]);
  for (size_t i = 1; i < operands.size(); i++) {
    auto &this_shape = inner_->shape(operands[i]);
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
      .holder = base_type.holder,
      .shape = push(std::move(new_shape)),
  };
  return push_op(ConcateOp{
      .type = new_type,
      .operands = std::move(operands),
      .dimension = dimension,
  });
}

auto FluxBuilder::check_shape(OpHandle x, OpHandle y) const -> bool {
  return inner_->type(x).shape == inner_->type(y).shape;
}

auto FluxBuilder::check_holder(OpHandle x, OpHandle y) const -> bool {
  return inner_->type(x).holder == inner_->type(y).holder;
}

} // namespace fastmpc::flux