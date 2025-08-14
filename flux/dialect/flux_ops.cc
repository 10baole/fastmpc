#include "fastmpc/flux/dialect/flux_ops.h"

#include <ostream>

#include "fastmpc/flux/dialect/flux_context.h"

namespace fastmpc::flux {

namespace {

void print_attr(std::ostream &out, const char *name, size_t attr) {
  out << name << " = " << attr;
}

void print_attr(std::ostream &out, const char *name, uint8_t attr) {
  out << name << " = " << static_cast<size_t>(attr);
}

template <class Attr>
void print_attr(std::ostream &out, const char *name, const Attr &attr) {
  out << name << " = [";
  attr.print(out);
  out << ']';
}
} // namespace

void OpHandle::print(std::ostream &out) const { out << '%' << handle_; }

void InputOp::print(std::ostream &out, const FluxContext &context) const {
  out << "input ";
  print_attr(out, "input_index", input_index);
  out << ", ";
  print_attr(out, "tuple_index", tuple_index);
  out << " : ";
  type.print(out, context);
}

void OutputOp::print(std::ostream &out, const FluxContext &context) const {
  out << "output ";
  operand.print(out);
  out << ", ";
  print_attr(out, "output_index", output_index);
  out << ", ";
  print_attr(out, "tuple_index", tuple_index);
  out << " : ";
  context.type(operand).print(out, context);
}

#define DEF_BINARY_OP(OpName, Name)                                            \
  void OpName::print(std::ostream &out, const FluxContext &context) const {    \
    out << #Name " ";                                                          \
    left.print(out);                                                           \
    out << ", ";                                                               \
    right.print(out);                                                          \
    out << ": (";                                                              \
    context.type(left).print(out, context);                                    \
    out << ", ";                                                               \
    context.type(right).print(out, context);                                   \
    out << ") -> ";                                                            \
    type.print(out, context);                                                  \
  }
DEF_BINARY_OP(AddOp, add)
DEF_BINARY_OP(AndOp, and);
DEF_BINARY_OP(MatmulOp, matmul)
DEF_BINARY_OP(MultiplyOp, multiply)
DEF_BINARY_OP(SubtractOp, subtract)
DEF_BINARY_OP(XorOp, xor)
#undef DEF_BINARY_OP

#define DEF_UNARY_OP(OpName, Name)                                             \
  void OpName::print(std::ostream &out, const FluxContext &context) const {    \
    out << #Name " ";                                                          \
    operand.print(out);                                                        \
    out << " : (";                                                             \
    context.type(operand).print(out, context);                                 \
    out << ") -> ";                                                            \
    type.print(out, context);                                                  \
  }
DEF_UNARY_OP(BitReverseOp, bit_reverse)
DEF_UNARY_OP(CastOp, cast)
DEF_UNARY_OP(NegateOp, negate)
DEF_UNARY_OP(NotOp, not )
DEF_UNARY_OP(ReshapeOp, reshape)
#undef DEF_UNARY_OP

namespace {

auto unwarped(uint8_t bits, const FluxContext &) { return bits; }

auto unwarped(DenseSizeTHandle handle, const FluxContext &context) {
  return context.dense_size_t(handle);
}

} // namespace

#define DEF_UNARY_OP(OpName, Name, Attr)                                       \
  void OpName::print(std::ostream &out, const FluxContext &context) const {    \
    out << #Name " ";                                                          \
    operand.print(out);                                                        \
    out << ", ";                                                               \
    print_attr(out, #Attr, unwarped(Attr, context));                           \
    out << " : (";                                                             \
    context.type(operand).print(out, context);                                 \
    out << ") -> ";                                                            \
    type.print(out, context);                                                  \
  }
DEF_UNARY_OP(AShiftRightOp, arith_shift_right, bits)
DEF_UNARY_OP(BroadcastOp, broadcast, dimensions)
DEF_UNARY_OP(InverseOp, inverse, fixed_point)
DEF_UNARY_OP(LShiftRightOp, logic_shift_right, bits)
DEF_UNARY_OP(TransposeOp, transpose, permutation)
DEF_UNARY_OP(ShiftLeftOp, shift_left, bits)
#undef DEF_UNARY_OP

void SliceOp::print(std::ostream &out, const FluxContext &context) const {
  out << "slice ";
  operand.print(out);
  out << ", ";
  print_attr(out, "start", context.dense_size_t(start));
  out << ", ";
  print_attr(out, "end", context.dense_size_t(end));
  out << ", ";
  print_attr(out, "stride", context.dense_size_t(stride));
  out << " : (";
  context.type(operand).print(out, context);
  out << ") -> ";
  type.print(out, context);
}

void ConcateOp::print(std::ostream &out, const FluxContext &context) const {
  out << "concate ";
  operands[0].print(out);
  for (size_t i = 1; i < operands.size(); i++) {
    out << ", ";
    operands[i].print(out);
  }
  out << ", ";
  print_attr(out, "dimension", dimension);
  out << " : (";
  context.type(operands[0]).print(out, context);
  for (size_t i = 1; i < operands.size(); i++) {
    out << ", ";
    context.type(operands[i]).print(out, context);
  }
  out << ") -> ";
  type.print(out, context);
}

void ConstantOp::print(std::ostream &out, const FluxContext &context) const {
  out << "constant ";
  print_attr(out, "value", context.dense_value(value));
  out << " : ";
  type.print(out, context);
}

void RandomOp::print(std::ostream &out, const FluxContext &context) const {
  out << "random ";
  print_attr(out, "rng_index", rng_index);
  out << ", ";
  print_attr(out, "rng_seed", rng_seed);
  out << " : ";
  type.print(out, context);
}

} // namespace fastmpc::flux