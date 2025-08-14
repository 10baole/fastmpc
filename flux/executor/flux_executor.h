#pragma once

#include <cassert>
#include <cstddef>
#include <map>
#include <utility>

#include "fastmpc/eager/tensor.h"
#include "fastmpc/flux/dialect/flux_context.h"
#include "fastmpc/flux/dialect/flux_ops.h"

namespace fastmpc::flux {

class FluxExecutor : public ValuePrinter {
public:
  explicit FluxExecutor(const FluxContext &context) : context_(&context) {}

  auto input(size_t input_index, size_t tuple_index) -> eager::Tensor & {
    return inputs_[{input_index, tuple_index}];
  }

  auto output(size_t output_index, size_t tuple_index) -> eager::Tensor & {
    auto it = outputs_.find({output_index, tuple_index});
    assert(it != outputs_.end());
    return it->second;
  }

  void run();

  void operator()(OpHandle handle, InputOp op);
  void operator()(OpHandle handle, OutputOp op);
  void operator()(OpHandle handle, AShiftRightOp op);
  void operator()(OpHandle handle, BitReverseOp op);
  void operator()(OpHandle handle, BroadcastOp op);
  void operator()(OpHandle handle, CastOp op);
  void operator()(OpHandle handle, InverseOp op);
  void operator()(OpHandle handle, LShiftRightOp op);
  void operator()(OpHandle handle, NegateOp op);
  void operator()(OpHandle handle, NotOp op);
  void operator()(OpHandle handle, ReshapeOp op);
  void operator()(OpHandle handle, ShiftLeftOp op);
  void operator()(OpHandle handle, SliceOp op);
  void operator()(OpHandle handle, TransposeOp op);
  void operator()(OpHandle handle, AddOp op);
  void operator()(OpHandle handle, AndOp op);
  void operator()(OpHandle handle, MatmulOp op);
  void operator()(OpHandle handle, MultiplyOp op);
  void operator()(OpHandle handle, SubtractOp op);
  void operator()(OpHandle handle, XorOp op);
  void operator()(OpHandle handle, ConstantOp op);
  void operator()(OpHandle handle, RandomOp op);
  void operator()(OpHandle handle, ConcateOp op);

private:
  auto get(OpHandle key) -> eager::Tensor;
  void push(OpHandle key, eager::Tensor value);
  void print_value(std::ostream &out, OpHandle handle) override;

  std::map<OpHandle, eager::Tensor> map_;
  std::map<std::pair<size_t, size_t>, eager::Tensor> inputs_;
  std::map<std::pair<size_t, size_t>, eager::Tensor> outputs_;
  const FluxContext *const context_;
};

} // namespace fastmpc::flux