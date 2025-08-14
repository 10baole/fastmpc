#pragma once

#include <cstddef>
#include <map>
#include <vector>

#include "fastmpc/abp/dialect/abp_context.h"
#include "fastmpc/abp/dialect/abp_ops.h"
#include "fastmpc/eager/tensor.h"
#include "fastmpc/eager/tensor_ops.h"

namespace fastmpc::abp {

class ABPExecutor : public ValuePrinter {
public:
  ABPExecutor(const ABPContext &context, size_t input_size, size_t output_size)
      : context_(&context), intputs_(input_size), outputs_(output_size) {}

  auto &input(size_t index) { return intputs_[index]; }
  auto &output(size_t index) { return outputs_[index]; }
  void run();

  void operator()(OpHandle handle, InputOp op);
  void operator()(OpHandle handle, OutputOp op);
  void operator()(OpHandle handle, ConstantOp op);
  void operator()(OpHandle handle, NegateAOp op);
  void operator()(OpHandle handle, NegatePOp op);
  void operator()(OpHandle handle, InverseOp op);
  void operator()(OpHandle handle, TruncateAOp op);
  void operator()(OpHandle handle, TruncatePOp op);
  void operator()(OpHandle handle, NotBOp op);
  void operator()(OpHandle handle, BitReverseOp op);
  void operator()(OpHandle handle, ShiftRightOp op);
  void operator()(OpHandle handle, P2AOp op);
  void operator()(OpHandle handle, A2BOp op);
  void operator()(OpHandle handle, B2AOp op);
  void operator()(OpHandle handle, BroadcastOp op);
  void operator()(OpHandle handle, ReshapeOp op);
  void operator()(OpHandle handle, SliceOp op);
  void operator()(OpHandle handle, TransposeOp op);
  void operator()(OpHandle handle, AddAAOp op);
  void operator()(OpHandle handle, AddAPOp op);
  void operator()(OpHandle handle, AddPPOp op);
  void operator()(OpHandle handle, MultiplyAAOp op);
  void operator()(OpHandle handle, MultiplyAPOp op);
  void operator()(OpHandle handle, MultiplyPPOp op);
  void operator()(OpHandle handle, XorBBOp op);
  void operator()(OpHandle handle, AndBBOp op);
  void operator()(OpHandle handle, DotGeneralAAOp op);
  void operator()(OpHandle handle, ConcateOp op);
  void print_value(std::ostream &out, OpHandle handle) override;

private:
  const ABPContext *const context_;
  std::vector<eager::Tensor> intputs_;
  std::vector<eager::Tensor> outputs_;
  std::map<OpHandle, eager::Tensor> map_;
};

} // namespace fastmpc::abp