#pragma once

#include <map>
#include <utility>

#include "fastmpc/flux/dialect/flux_context.h"
#include "fastmpc/flux/dialect/flux_ops.h"
#include "fastmpc/flux/dialect/flux_types.h"
#include "fastmpc/ir_base/attribute.h"

namespace fastmpc::flux {

class FluxBuilder {
public:
  explicit FluxBuilder(FluxContext &context) : inner_(&context) {}
  auto &context() const { return *inner_; }

  auto input(size_t input_index, size_t tuple_index, Type type) -> OpHandle;
  auto output(OpHandle operand, size_t output_index, size_t tuple_index)
      -> OpHandle;
  auto arith_shift_right(OpHandle operand, uint8_t bits) -> OpHandle;
  auto bit_reverse(OpHandle operand) -> OpHandle;
  auto broadcast(OpHandle operand, DenseSizeT &&dimensions, ShapeHandle shape)
      -> OpHandle;
  auto broadcast(OpHandle operand, DenseSizeTHandle dimensions,
                 ShapeHandle shape) -> OpHandle;
  auto cast(OpHandle operand, size_t holder) -> OpHandle;
  auto inverse(OpHandle operand, uint8_t fixed_point) -> OpHandle;
  auto logic_shift_right(OpHandle operand, uint8_t bits) -> OpHandle;

  auto negate(OpHandle operand) -> OpHandle;
  auto _not(OpHandle operand) -> OpHandle;
  auto reshape(OpHandle operand, ShapeHandle shape) -> OpHandle;
  auto shift_left(OpHandle operand, uint8_t bits) -> OpHandle;
  auto slice(OpHandle operand, DenseSizeTHandle start, DenseSizeTHandle end,
             DenseSizeTHandle strides) -> OpHandle;
  auto transpose(OpHandle opearnd, DenseSizeTHandle permutation) -> OpHandle;
  auto add(OpHandle left, OpHandle right) -> OpHandle;
  auto _and(OpHandle left, OpHandle right) -> OpHandle;
  auto matmul(OpHandle left, OpHandle right) -> OpHandle;
  auto multiply(OpHandle left, OpHandle right) -> OpHandle;
  auto subtract(OpHandle left, OpHandle right) -> OpHandle;
  auto _xor(OpHandle left, OpHandle right) -> OpHandle;
  auto constant(DenseValueHandle value, Type type) -> OpHandle;
  auto constant(DenseValue &&value, Type type) -> OpHandle;
  auto random(size_t p0, size_t p1, ShapeHandle shape)
      -> std::pair<OpHandle, OpHandle>;

  auto concate(std::vector<OpHandle> &&operands, size_t dimension) -> OpHandle;

  template <class T> auto push(T &&value) -> typename T::handle_type;

private:
  template <class Op> auto push_op(Op &&op) -> OpHandle;

  auto check_holder(OpHandle left, OpHandle right) const -> bool;
  auto check_shape(OpHandle left, OpHandle right) const -> bool;

  FluxContext *inner_;
  std::map<std::pair<size_t, size_t>, size_t> rng_seed_;
};

template <> inline auto FluxBuilder::push<Shape>(Shape &&value) -> ShapeHandle {
  return inner_->shape_list_.push(~value);
}

template <>
inline auto FluxBuilder::push<DenseSizeT>(DenseSizeT &&item)
    -> DenseSizeTHandle {
  return inner_->dense_size_t_list_.push(~item);
}

template <>
inline auto FluxBuilder::push<DenseValue>(DenseValue &&item)
    -> DenseValueHandle {
  return inner_->dense_value_list_.push(~item);
}

} // namespace fastmpc::flux