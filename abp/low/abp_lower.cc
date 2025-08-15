#include "fastmpc/abp/low/abp_lower.h"

#include <cassert>
#include <utility>
#include <vector>

#include "fastmpc/abp/dialect/abp_ops.h"
#include "fastmpc/abp/function/abp_binary.h"
#include "fastmpc/abp/function/abp_compare.h"
#include "fastmpc/abp/function/abp_constant.h"
#include "fastmpc/abp/function/abp_function.h"
#include "fastmpc/pphlo/pphlo_context.h"
#include "libspu/dialect/pphlo_ops.h"
#include "libspu/dialect/pphlo_types.h"
#include "mlir/Dialect/Func/IR/FuncOps.h"
#include "mlir/IR/AttrTypeSubElements.h"
#include "mlir/IR/BuiltinAttributes.h"
#include "mlir/IR/Operation.h"
#include "mlir/IR/Region.h"
#include "mlir/IR/Types.h"
#include "llvm/Support/Casting.h"

namespace fastmpc::abp {

namespace {
auto shape_of(mlir::Type base_type) {
  auto type = base_type.dyn_cast<mlir::ShapedType>();
  auto shape = type.getShape();
  return Shape(shape.begin(), shape.end());
}

auto is_public(mlir::Type base_type) {
  mlir::pphlo::TypeTools tool;
  return tool.isMPCType<mlir::pphlo::PublicType>(base_type);
}

auto base_type(mlir::Type type) {
  mlir::pphlo::TypeTools tool;
  return tool.getExpressedType(type);
}

auto collect(const mlir::DenseIntElementsAttr &attr) {
  auto values = attr.getValues<size_t>();
  return DenseSizeT(values.begin(), values.end());
}
} // namespace

void ABPLower::run() {
  auto entry = source_->module().lookupSymbol<mlir::func::FuncOp>("main");
  auto arguments = entry.getRegion().getArguments();
  for (auto argument : arguments) {
    size_t input_index = argument.getArgNumber();
    auto mlir_type = argument.getType();
    TypeKind kind =
        is_public(mlir_type) ? TypeKind::kFixed64 : TypeKind::kArithFixed64;
    auto shape = shape_of(mlir_type);
    uint8_t fixed_point = 0;
    if (base_type(mlir_type).isF32()) {
      fixed_point = builder_->fixed_point();
    } else {
      assert(base_type(mlir_type).isIntOrIndex());
    }
    Type type{
        .kind = kind,
        .fixed_point = fixed_point,
        .shape = builder_->push(~shape),
    };
    auto handle = builder_->input(input_index, type);
    map_.try_emplace(argument, handle);
  }
  low(&entry.getBody());
}

void ABPLower::low(mlir::Region *region) {
  assert(region->hasOneBlock());
  return low(&region->front());
}

void ABPLower::low(mlir::Block *block) {
  for (auto &op : block->getOperations()) {
    low(&op);
  }
}

void ABPLower::low_return(mlir::func::ReturnOp *op) {
  size_t output_index = 0;
  for (auto operand : op->getOperands()) {
    auto handle = map_.find(operand)->second;
    builder_->output(handle, output_index++);
  }
}

void ABPLower::low(mlir::Operation *in) {
#define cast(OpName) llvm::dyn_cast<mlir::pphlo::OpName>(in)
  if (auto op = cast(AbsOp)) {
    low_abs(&op);
  } else if (auto op = cast(AddOp)) {
    low_add(&op);
  } else if (auto op = cast(BroadcastOp)) {
    low_broadcast(&op);
  } else if (auto op = cast(ConcatenateOp)) {
    low_concatenate(&op);
  } else if (auto op = cast(ConstantOp)) {
    low_constant(&op);
  } else if (auto op = cast(ConvertOp)) {
    low_convert(&op);
  } else if (auto op = cast(ConvolutionOp)) {
    low_convolution(&op);
  } else if (auto op = cast(DivOp)) {
    low_divide(&op);
  } else if (auto op = cast(DotGeneralOp)) {
    low_dot_general(&op);
  } else if (auto op = cast(EqualOp)) {
    low_equal(&op);
  } else if (auto op = cast(ExpOp)) {
    low_exponential(&op);
  } else if (auto op = cast(GreaterOp)) {
    low_greater(&op);
  } else if (auto op = cast(GreaterEqualOp)) {
    low_greater_equal(&op);
  } else if (auto op = cast(IotaOp)) {
    low_iota(&op);
  } else if (auto op = cast(LessOp)) {
    low_less(&op);
  } else if (auto op = cast(LogOp)) {
    low_log(&op);
  } else if (auto op = cast(MaxOp)) {
    low_maximum(&op);
  } else if (auto op = cast(MulOp)) {
    low_multiply(&op);
  } else if (auto op = cast(NegOp)) {
    low_negate(&op);
  } else if (auto op = cast(PadOp)) {
    low_pad(&op);
  } else if (auto op = cast(PowOp)) {
    low_power(&op);
  } else if (auto op = cast(ReduceOp)) {
    low_reduce(&op);
  } else if (auto op = cast(ReduceWindowOp)) {
    low_reduce_window(&op);
  } else if (auto op = cast(ReshapeOp)) {
    low_reshape(&op);
  } else if (auto op = cast(ReturnOp)) {
    low_return(&op);
  } else if (auto op = cast(ReverseOp)) {
    low_reverse(&op);
  } else if (auto op = cast(RsqrtOp)) {
    low_rsqrt(&op);
  } else if (auto op = cast(SelectOp)) {
    low_select(&op);
  } else if (auto op = cast(SelectAndScatterOp)) {
    low_select_and_scatter(&op);
  } else if (auto op = cast(SliceOp)) {
    low_slice(&op);
  } else if (auto op = cast(SqrtOp)) {
    low_sqrt(&op);
  } else if (auto op = cast(SubtractOp)) {
    low_subtract(&op);
  } else if (auto op = cast(TransposeOp)) {
    low_transpose(&op);
  } else if (auto op = llvm::dyn_cast<mlir::func::ReturnOp>(in)) {
    low_return(&op);
  } else {
    unsupported(op);
  }
#undef cast
}

void ABPLower::low_abs(mlir::pphlo::AbsOp *op) {
  auto operand = map_.find(op->getOperand())->second;
  auto result = abs(*builder_, operand);
  map_.try_emplace(op->getResult(), result);
}

void ABPLower::low_add(mlir::pphlo::AddOp *op) {
  auto left = map_.find(op->getOperand(0))->second;
  auto right = map_.find(op->getOperand(1))->second;
  auto result = add(*builder_, left, right);
  map_.try_emplace(op->getResult(), result);
}

void ABPLower::low_subtract(mlir::pphlo::SubtractOp *op) {
  auto left = map_.find(op->getOperand(0))->second;
  auto right = map_.find(op->getOperand(1))->second;
  auto result = subtract(*builder_, left, right);
  map_.try_emplace(op->getResult(), result);
}

void ABPLower::low_multiply(mlir::pphlo::MulOp *op) {
  auto left = map_.find(op->getOperand(0))->second;
  auto right = map_.find(op->getOperand(1))->second;
  auto result = multiply(*builder_, left, right);
  map_.try_emplace(op->getResult(), result);
}

void ABPLower::low_negate(mlir::pphlo::NegOp *op) {
  auto operand = map_.find(op->getOperand())->second;
  auto result = negate(*builder_, operand);
  map_.try_emplace(op->getResult(), result);
}

namespace {

template <class In, class Out>
auto warp(ABPBuilder &builder, mlir::DenseElementsAttr &data, Shape &&shape) {
  if (data.isSplat()) {
    auto value = static_cast<Out>(data.getSplatValue<In>());
    return constant(builder, value);
  } else {
    std::vector<Out> value(data.getNumElements());
    for (const auto &v : llvm::enumerate(data.getValues<In>())) {
      value[v.index()] = static_cast<Out>(v.value());
    }
    return constant(builder, std::move(value), ~shape);
  }
}

} // namespace

void ABPLower::low_constant(mlir::pphlo::ConstantOp *op) {
  auto value = op->getValue();
  auto type = value.getType();
  auto data = value.dyn_cast<mlir::DenseElementsAttr>();
  auto shape = shape_of(type);
  auto data_type = data.getElementType();

  OpHandle result(0);
  if (data_type.isInteger(1)) {
    result = warp<bool, uint64_t>(*builder_, data, ~shape);
  } else if (data_type.isInteger(32)) {
    result = warp<int, uint64_t>(*builder_, data, ~shape);
  } else if (data_type.isF32()) {
    result = warp<float, float>(*builder_, data, ~shape);
  } else {
    std::abort();
  }
  map_.try_emplace(op->getResult(), result);
}

void ABPLower::low_convert(mlir::pphlo::ConvertOp *op) {
  auto &context = builder_->context();
  auto operand = op->getOperand();
  auto input_type = operand.getType();
  auto output_type = op->getType();

  auto input = map_.find(operand)->second;
  if (is_public(input_type) == is_public(output_type)) {
    assert(base_type(input_type).isIntOrIndex());
    assert(base_type(output_type).isF32());
    auto input_shape = context.type(input).shape;
    auto one = constant(*builder_, 1.f);
    if (!context.shape(input_shape).empty()) {
      one = builder_->broadcast(one, {}, input_shape);
    }
    auto result = multiply(*builder_, input, one);
    map_.try_emplace(op->getResult(), result);
  } else {
    assert(is_public(input_type));
    map_.try_emplace(op->getResult(), builder_->p2a(input));
  }
}

void ABPLower::low_equal(mlir::pphlo::EqualOp *op) {
  auto left = map_.find(op->getLhs())->second;
  auto right = map_.find(op->getRhs())->second;
  auto result = isEqual(*builder_, left, right);
  map_.try_emplace(op->getResult(), result);
}

void ABPLower::low_greater(mlir::pphlo::GreaterOp *op) {
  auto left = map_.find(op->getLhs())->second;
  auto right = map_.find(op->getRhs())->second;
  auto result = isGreater(*builder_, left, right);
  map_.try_emplace(op->getResult(), result);
}

void ABPLower::low_greater_equal(mlir::pphlo::GreaterEqualOp *op) {
  auto left = map_.find(op->getLhs())->second;
  auto right = map_.find(op->getRhs())->second;
  auto result = isGEQ(*builder_, left, right);
  map_.try_emplace(op->getResult(), result);
}

void ABPLower::low_less(mlir::pphlo::LessOp *op) {
  auto left = map_.find(op->getLhs())->second;
  auto right = map_.find(op->getRhs())->second;
  auto result = isLess(*builder_, left, right);
  map_.try_emplace(op->getResult(), result);
}

void ABPLower::low_broadcast(mlir::pphlo::BroadcastOp *op) {
  auto operand = map_.find(op->getOperand())->second;
  auto dimensions = collect(op->getBroadcastDimensions());
  auto shape = builder_->push(shape_of(op->getType()));
  auto result = builder_->broadcast(operand, ~dimensions, shape);
  map_.try_emplace(op->getResult(), result);
}

void ABPLower::low_reshape(mlir::pphlo::ReshapeOp *op) {
  auto operand = map_.find(op->getOperand())->second;
  auto shape = builder_->push(shape_of(op->getType()));
  auto result = builder_->reshape(operand, shape);
  map_.try_emplace(op->getResult(), result);
}

void ABPLower::low_select(mlir::pphlo::SelectOp *op) {

  auto pred = map_.find(op->getPred())->second;
  auto on_true = map_.find(op->getOnTrue())->second;
  auto on_false = map_.find(op->getOnFalse())->second;
  auto result = selectOne(*builder_, pred, on_true, on_false);
  map_.try_emplace(op->getResult(), result);
}

void ABPLower::low_maximum(mlir::pphlo::MaxOp *op) {
  auto left = map_.find(op->getLhs())->second;
  auto right = map_.find(op->getRhs())->second;
  auto result = maximize(*builder_, left, right);
  map_.try_emplace(op->getResult(), result);
}

void ABPLower::low_iota(mlir::pphlo::IotaOp *op) {
  auto iota_dimension = op->getIotaDimension();
  auto shape = shape_of(op->getType());
  auto type = op->getType();
  auto result = iota(*builder_, iota_dimension, ~shape);

  if (!base_type(type).isF32()) {
    assert(base_type(type).isIntOrIndex());
  } else {
    auto one = constant(*builder_, 1.f);
    auto result_shape = context().type(one).shape;
    if (!context().shape(result_shape).empty()) {
      one = builder_->broadcast(one, {}, result_shape);
    }
    result = multiply(*builder_, result, one);
  }
  map_.try_emplace(op->getResult(), result);
}

void ABPLower::low_transpose(mlir::pphlo::TransposeOp *op) {
  auto operand = map_.find(op->getOperand())->second;
  auto permutation = collect(op->getPermutation());
  auto result = builder_->transpose(operand, ~permutation);
  map_.try_emplace(op->getResult(), result);
}

void ABPLower::low_divide(mlir::pphlo::DivOp *op) {
  auto left = map_.find(op->getOperand(0))->second;
  auto right = map_.find(op->getOperand(1))->second;
  auto result = divide(*builder_, left, right);
  map_.try_emplace(op->getResult(), result);
}

void ABPLower::low_exponential(mlir::pphlo::ExpOp *op) {
  auto operand = map_.find(op->getOperand())->second;
  auto result = exp(*builder_, operand);
  map_.try_emplace(op->getResult(), result);
}

// Warning: I have hacked pphlo.BroadcastOp's lowing function to make this code
// work!!!
void ABPLower::low_dot_general(mlir::pphlo::DotGeneralOp *op) {
  auto &context = builder_->context();
  auto dim_numbers = op->getDotDimensionNumbers();
  auto lhs = map_.find(op->getLhs())->second;
  auto rhs = map_.find(op->getRhs())->second;
  auto left_type = context.type(lhs);
  auto right_type = context.type(rhs);
  auto &left_shape = context.shape(left_type.shape);
  auto &right_shape = context.shape(right_type.shape);
  assert(left_shape.size() == 2);
  assert(right_shape.size() == 2);
  assert(dim_numbers.getLhsContractingDimensions().size() == 1);
  assert(dim_numbers.getRhsContractingDimensions().size() == 1);
  size_t left_contract = dim_numbers.getLhsContractingDimensions()[0];
  size_t right_contract = dim_numbers.getRhsContractingDimensions()[0];

  if (left_contract == 0) {
    lhs = builder_->transpose(lhs, {1, 0});
  }
  if (right_contract == 1) {
    rhs = builder_->transpose(rhs, {1, 0});
  }
  auto result = dot_general(*builder_, lhs, rhs);
  map_.try_emplace(op->getResult(), result);
}

void ABPLower::low_log(mlir::pphlo::LogOp *op) {
  auto operand = map_.find(op->getOperand())->second;
  auto result = log(*builder_, operand);
  map_.try_emplace(op->getResult(), result);
}

void ABPLower::low_slice(mlir::pphlo::SliceOp *op) {
  auto operand = map_.find(op->getOperand())->second;
  auto start = collect(op->getStartIndices());
  auto end = collect(op->getLimitIndices());
  auto stride = collect(op->getStrides());
  auto result = builder_->slice(operand, ~start, ~end, ~stride);
  map_.try_emplace(op->getResult(), result);
}

void ABPLower::low_concatenate(mlir::pphlo::ConcatenateOp *op) {
  std::vector<OpHandle> operands;
  operands.reserve(op->getNumOperands());
  for (auto operand : op->getOperands()) {
    operands.push_back(map_.find(operand)->second);
  }
  size_t dim = op->getDimension();
  auto result = builder_->concate(std::move(operands), dim);
  map_.try_emplace(op->getResult(), result);
}

void ABPLower::low_reduce(mlir::pphlo::ReduceOp *op) {
  assert(op->getNumResults() == 1);
  auto reduce_dims = collect(op->getDimensions());
  std::vector<OpHandle> inputs;
  for (auto value : op->getInputs()) {
    inputs.push_back(map_.find(value)->second);
  }
  std::vector<OpHandle> inits;
  for (auto value : op->getInitValues()) {
    inits.push_back(map_.find(value)->second);
  }
  assert(reduce_dims.size() == 1);
  assert(inputs.size() == 1);
  assert(inits.size() == 1);

  auto &block = op->getBody().front();
  assert(block.getNumArguments() == 2);

  ReduceFunc reducer = nullptr;
  for (auto &in : block.without_terminator()) {
    assert(reducer == nullptr);
    if (auto op = llvm::dyn_cast<mlir::pphlo::AddOp>(in)) {
      reducer = add;
    } else if (auto op = llvm::dyn_cast<mlir::pphlo::MaxOp>(in)) {
      reducer = maximize;
    } else {
      llvm::errs() << op->getName() << " is not supported in reduceOp!\n";
      std::abort();
    }
  }
  auto result = reduce(*builder_, inputs[0], inits[0], ~reduce_dims,
                       shape_of(op->getType(0)), reducer);
  map_.try_emplace(op->getResult(0), result);
}

void ABPLower::unsupported(mlir::Operation *op) {
  llvm::errs() << op->getName() << " is not supported!\n";
  std::abort();
}

#define MARK_UNSUPPORTED(FuncName, OpName)                                     \
  void ABPLower::FuncName(mlir::pphlo::OpName *op) { unsupported(*op); }
MARK_UNSUPPORTED(low_convolution, ConvolutionOp)
MARK_UNSUPPORTED(low_pad, PadOp)
MARK_UNSUPPORTED(low_power, PowOp)
MARK_UNSUPPORTED(low_reduce_window, ReduceWindowOp)
MARK_UNSUPPORTED(low_return, ReturnOp) // today
MARK_UNSUPPORTED(low_reverse, ReverseOp)
MARK_UNSUPPORTED(low_rsqrt, RsqrtOp)
MARK_UNSUPPORTED(low_select_and_scatter, SelectAndScatterOp)
MARK_UNSUPPORTED(low_sqrt, SqrtOp)

} // namespace fastmpc::abp