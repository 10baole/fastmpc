#include "fastmpc/abp/dialect/abp_ops.h"

#include <cstddef>
#include <ostream>

#include "fastmpc/abp/dialect/abp_context.h"
#include "fastmpc/abp/dialect/abp_types.h"
#include "fastmpc/ir_base/fvn_context.h"

namespace fastmpc::abp {

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

auto Op::hash() const -> size_t { 
  FVNContext context;
  context.push(static_cast<size_t>(kind));
  context.push(offset);
  return context.value();
}

void OpHandle::print(std::ostream &out) const { out << '%' << handle_; }

void InputOp::print(std::ostream &out, const ABPContext &context) const {
  out << "input ";
  print_attr(out, "input_index", input_index);
  out << " : ";
  type.print(out, context);
}

void OutputOp::print(std::ostream &out, const ABPContext &context) const {
  out << "output ";
  operand.print(out);
  out << ", ";
  print_attr(out, "output_index", output_index);
  out << " : ";
  context.type(operand).print(out, context);
}

auto ConstantOp::hash() const -> size_t {
  FVNContext context;
  context.push(type.hash());
  context.push(value.unwarp());
  return context.value();
}

void ConstantOp::print(std::ostream &out, const ABPContext &context) const {
  out << "constant ";
  print_attr(out, "value", context.dense_value(value));
  out << " : ";
  type.print(out, context);
}

auto ConstantOp::operator==(const ConstantOp &other) const -> bool {
  return type == other.type && value == other.value;
}

#define DEF_UNARY_OP(OpName, Name)                                             \
  auto OpName::hash() const->size_t {                                          \
    FVNContext context;                                                        \
    context.push(type.hash());                                                 \
    context.push(operand);                                                     \
    return context.value();                                                    \
  }                                                                            \
  void OpName::print(std::ostream &out, const ABPContext &context) const {     \
    out << #Name << ' ';                                                       \
    operand.print(out);                                                        \
    out << " : (";                                                             \
    context.type(operand).print(out, context);                                 \
    out << ") -> ";                                                            \
    type.print(out, context);                                                  \
  }                                                                            \
  auto OpName::operator==(const OpName &other) const->bool {                   \
    return operand == other.operand;                                           \
  }
DEF_UNARY_OP(NegateAOp, negate_a)
DEF_UNARY_OP(NegatePOp, negate_p)
DEF_UNARY_OP(InverseOp, inverse)
DEF_UNARY_OP(NotBOp, not_b)
DEF_UNARY_OP(BitReverseOp, bit_reverse)
DEF_UNARY_OP(P2AOp, p2a)
DEF_UNARY_OP(A2BOp, a2b)
DEF_UNARY_OP(B2AOp, b2a)
DEF_UNARY_OP(ReshapeOp, reshape)
#undef DEF_UNARY_OP

namespace {

auto unwarped(uint8_t bits, const ABPContext &) { return bits; }

auto unwarped(DenseSizeTHandle handle, const ABPContext &context) {
  return context.dense_size_t(handle);
}

} // namespace

#define DEF_UNARY_OP(OpName, Name, Attr)                                       \
  auto OpName::hash() const->size_t {                                          \
    FVNContext context;                                                        \
    context.push(type.hash());                                                 \
    context.push(operand.unwarp());                                            \
    context.push(Attr);                                                        \
    return context.value();                                                    \
  }                                                                            \
  void OpName::print(std::ostream &out, const ABPContext &context) const {     \
    out << #Name << ' ';                                                       \
    operand.print(out);                                                        \
    out << ", ";                                                               \
    print_attr(out, #Attr, unwarped(Attr, context));                           \
    out << " : (";                                                             \
    context.type(operand).print(out, context);                                 \
    out << ") -> ";                                                            \
    type.print(out, context);                                                  \
  }                                                                            \
  auto OpName::operator==(const OpName &other) const->bool {                   \
    return type == other.type && operand == other.operand &&                   \
           Attr == other.Attr;                                                 \
  }
DEF_UNARY_OP(ShiftRightOp, shift_right, bits)
DEF_UNARY_OP(TruncateAOp, truncate_a, bits)
DEF_UNARY_OP(TruncatePOp, truncate_p, bits)
DEF_UNARY_OP(BroadcastOp, broadcast, dimensions)
DEF_UNARY_OP(TransposeOp, transpose, permutation)
#undef DEF_UNARY_OP

auto SliceOp::hash() const -> size_t {
  FVNContext context;
  context.push(type.hash());
  context.push(operand);
  context.push(start);
  context.push(end);
  context.push(stride);
  return context.value();
}

void SliceOp::print(std::ostream &out, const ABPContext &context) const {
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

auto SliceOp::operator==(const SliceOp &other) const -> bool {
  return type == other.type && operand == other.operand &&
         start == other.start && end == other.end && stride == other.stride;
}

#define DEF_BINARY_OP(OpName, Name)                                            \
  auto OpName::hash() const->size_t {                                          \
    FVNContext context;                                                        \
    context.push(type.hash());                                                 \
    context.push(left);                                                        \
    context.push(right);                                                       \
    return context.value();                                                    \
  }                                                                            \
  void OpName::print(std::ostream &out, const ABPContext &context) const {     \
    out << #Name << ' ';                                                       \
    left.print(out);                                                           \
    out << ", ";                                                               \
    right.print(out);                                                          \
    out << ": (";                                                              \
    context.type(left).print(out, context);                                    \
    out << ", ";                                                               \
    context.type(right).print(out, context);                                   \
    out << ") -> ";                                                            \
    type.print(out, context);                                                  \
  }                                                                            \
  auto OpName::operator==(const OpName &other) const->bool {                   \
    return type == other.type && left == other.left && right == other.right;   \
  }
DEF_BINARY_OP(AddAAOp, add_aa)
DEF_BINARY_OP(AddAPOp, add_ap)
DEF_BINARY_OP(AddPPOp, add_pp)
DEF_BINARY_OP(MultiplyAAOp, multiply_aa)
DEF_BINARY_OP(MultiplyAPOp, multiply_ap)
DEF_BINARY_OP(MultiplyPPOp, multiply_pp)
DEF_BINARY_OP(XorBBOp, xor_bb)
DEF_BINARY_OP(AndBBOp, and_bb)
DEF_BINARY_OP(DotGeneralAAOp, dot_general)
#undef DEF_BINARY_OP

auto ConcateOp::hash() const -> size_t {
  FVNContext context;
  context.push(type.hash());
  context.push(operands.size());
  for (auto &operand : operands)
    context.push(operand);
  context.push(dimension);
  return context.value();
}

void ConcateOp::print(std::ostream &out, const ABPContext &context) const {
  out << "concate ";
  operands[0].print(out);
  for (size_t i = 1; i < operands.size(); i++) {
    out << ", ";
    operands[i].print(out);
  }
  out << ", ";
  print_attr(out, "dimension", dimension);
  out << ": (";
  context.type(operands[0]).print(out, context);
  for (size_t i = 1; i < operands.size(); i++) {
    out << ", ";
    context.type(operands[i]).print(out, context);
  }
  out << ") -> ";
  type.print(out, context);
}

auto ConcateOp::operator==(const ConcateOp &other) const -> bool {
  return type == other.type && operands == other.operands &&
         dimension == other.dimension;
}

} // namespace fastmpc::abp