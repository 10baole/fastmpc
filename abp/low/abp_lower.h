#pragma once

#include "fastmpc/abp/dialect/abp_builder.h"
#include "fastmpc/abp/dialect/abp_context.h"
#include "mlir/IR/Value.h"
#include "llvm/ADT/DenseMap.h"

namespace mlir {
class Block;
class Operation;
class Region;
} // namespace mlir

namespace mlir::func {
class ReturnOp;
}

namespace mlir::pphlo {
class AbsOp;
class AddOp;
class BroadcastOp;
class ConcatenateOp;
class ConstantOp;
class ConvertOp;
class ConvolutionOp;
class DivOp;
class DotGeneralOp;
class EqualOp;
class ExpOp;
class GreaterOp;
class GreaterEqualOp;
class IotaOp;
class LessOp;
class LogOp;
class MaxOp;
class MulOp;
class NegOp;
class PadOp;
class PowOp;
class ReduceOp;
class ReduceWindowOp;
class ReshapeOp;
class ReturnOp;
class ReverseOp;
class RsqrtOp;
class SelectOp;
class SelectAndScatterOp;
class SliceOp;
class SqrtOp;
class SubtractOp;
class TransposeOp;
} // namespace mlir::pphlo

namespace fastmpc::pphlo {
class PPHLOContext;
}

namespace fastmpc::abp {

class ABPLower {
public:
  ABPLower(const pphlo::PPHLOContext &source, ABPBuilder &builder)
      : source_(&source), builder_(&builder) {}

  void run();
  auto &context() { return builder_->context(); }

private:
  void low(mlir::Block *);
  void low(mlir::Region *);
  void low(mlir::Operation *);
  void low_return(mlir::func::ReturnOp *);
  void low_abs(mlir::pphlo::AbsOp *);
  void low_add(mlir::pphlo::AddOp *);
  void low_broadcast(mlir::pphlo::BroadcastOp *);
  void low_concatenate(mlir::pphlo::ConcatenateOp *);
  void low_constant(mlir::pphlo::ConstantOp *);
  void low_convert(mlir::pphlo::ConvertOp *);
  void low_convolution(mlir::pphlo::ConvolutionOp *);
  void low_divide(mlir::pphlo::DivOp *);
  void low_dot_general(mlir::pphlo::DotGeneralOp *);
  void low_equal(mlir::pphlo::EqualOp *);
  void low_exponential(mlir::pphlo::ExpOp *);
  void low_greater(mlir::pphlo::GreaterOp *);
  void low_greater_equal(mlir::pphlo::GreaterEqualOp *);
  void low_iota(mlir::pphlo::IotaOp *);
  void low_less(mlir::pphlo::LessOp *);
  void low_log(mlir::pphlo::LogOp *);
  void low_maximum(mlir::pphlo::MaxOp *);
  void low_multiply(mlir::pphlo::MulOp *);
  void low_negate(mlir::pphlo::NegOp *);
  void low_pad(mlir::pphlo::PadOp *);
  void low_power(mlir::pphlo::PowOp *);
  void low_reduce(mlir::pphlo::ReduceOp *);
  void low_reduce_window(mlir::pphlo::ReduceWindowOp *);
  void low_reshape(mlir::pphlo::ReshapeOp *);
  void low_return(mlir::pphlo::ReturnOp *);
  void low_reverse(mlir::pphlo::ReverseOp *);
  void low_rsqrt(mlir::pphlo::RsqrtOp *);
  void low_select(mlir::pphlo::SelectOp *);
  void low_select_and_scatter(mlir::pphlo::SelectAndScatterOp *);
  void low_slice(mlir::pphlo::SliceOp *);
  void low_sqrt(mlir::pphlo::SqrtOp *);
  void low_subtract(mlir::pphlo::SubtractOp *);
  void low_transpose(mlir::pphlo::TransposeOp *);
  void unsupported(mlir::Operation *);

  const pphlo::PPHLOContext *const source_;
  ABPBuilder *builder_;
  llvm::DenseMap<mlir::Value, OpHandle> map_;
};

} // namespace fastmpc::abp