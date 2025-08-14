#include "fastmpc/flux/low/aby3/aby3_lower.h"

#include <cassert>

#include "fastmpc/abp/dialect/abp_ops.h"
#include "fastmpc/abp/dialect/abp_types.h"
#include "fastmpc/flux/low/3pc/3pc_value.h"
#include "fastmpc/flux/low/aby3/aby3_value.h"
#include "fastmpc/flux/low/aby3/function/aby3_binary.h"
#include "fastmpc/flux/low/aby3/function/aby3_casting.h"
#include "fastmpc/flux/low/aby3/function/aby3_unary.h"

namespace fastmpc::flux::aby3 {

auto ABY3Lower::get_cipher_value(abp::OpHandle handle) -> _3pc::CipherValue {
  auto value = map_.find(handle)->second;
  assert(value.kind == ValueKind::kCipherValue);
  return cast(cipher_values_[value.offset]);
}

auto ABY3Lower::get_plain_value(abp::OpHandle handle) -> _3pc::PlainValue {
  auto value = map_.find(handle)->second;
  assert(value.kind == ValueKind::kPlainValue);
  return cast(plain_values_[value.offset]);
}

void ABY3Lower::visit_value(abp::OpHandle handle, _3pc::ValueVisitor &visitor) {
  auto value = map_.find(handle)->second;
  switch (value.kind) {
  case ValueKind::kPlainValue:
    return visitor.visit(cast(plain_values_[value.offset]));
  case ValueKind::kCipherValue:
    return visitor.visit(cast(cipher_values_[value.offset]));
  }
}

void ABY3Lower::run() {
  for (size_t i = 0; i < abp_context_->ops_size(); i++) {
    abp::OpHandle handle(i);
    abp_context_->visit(handle, *this);
  }
}

void ABY3Lower::set_value(abp::OpHandle handle, _3pc::PlainValue value) {
  push(handle, cast(value));
}

void ABY3Lower::set_value(abp::OpHandle handle, _3pc::CipherValue value) {
  push(handle, cast(value));
}

void ABY3Lower::push(abp::OpHandle handle, PlainValue value) {
  size_t offset = plain_values_.size();
  plain_values_.push_back(value);
  map_.emplace(handle, Value{ValueKind::kPlainValue, offset});
}

void ABY3Lower::push(abp::OpHandle handle, CipherValue value) {
  size_t offset = cipher_values_.size();
  cipher_values_.push_back(value);
  map_.emplace(handle, Value{ValueKind::kCipherValue, offset});
}

void ABY3Lower::operator()(abp::OpHandle handle, abp::TruncateAOp op) {
  auto operand = get_cipher_value(op.operand);
  auto result = truncate_a(*builder_, cast(operand), op.bits);
  push(handle, result);
}

void ABY3Lower::operator()(abp::OpHandle handle, abp::A2BOp op) {
  auto operand = get_cipher_value(op.operand);
  auto result = a2b(*builder_, cast(operand));
  push(handle, result);
}

void ABY3Lower::operator()(abp::OpHandle handle, abp::B2AOp op) {
  auto operand = get_cipher_value(op.operand);
  auto result = b2a(*builder_, cast(operand));
  push(handle, result);
}

void ABY3Lower::operator()(abp::OpHandle handle, abp::AddAAOp op) {
  auto left = map_.find(op.left)->second;
  auto right = map_.find(op.right)->second;
  auto [left_value, right_value] = unpack_cc(left, right);
  push(handle, add_aa(*builder_, left_value, right_value));
}

void ABY3Lower::operator()(abp::OpHandle handle, abp::MultiplyAAOp op) {
  auto left = map_.find(op.left)->second;
  auto right = map_.find(op.right)->second;
  auto [left_value, right_value] = unpack_cc(left, right);
  push(handle, multiply_aa(*builder_, left_value, right_value));
}

void ABY3Lower::operator()(abp::OpHandle handle, abp::XorBBOp op) {
  auto left = map_.find(op.left)->second;
  auto right = map_.find(op.right)->second;
  auto [left_value, right_value] = unpack_cc(left, right);
  push(handle, xor_bb(*builder_, left_value, right_value));
}

void ABY3Lower::operator()(abp::OpHandle handle, abp::AndBBOp op) {
  auto left = map_.find(op.left)->second;
  auto right = map_.find(op.right)->second;
  auto [left_value, right_value] = unpack_cc(left, right);
  push(handle, and_bb(*builder_, left_value, right_value));
}

void ABY3Lower::operator()(abp::OpHandle handle, abp::DotGeneralAAOp op) {
  auto left = map_.find(op.left)->second;
  auto right = map_.find(op.right)->second;
  auto [left_value, right_value] = unpack_cc(left, right);
  push(handle, matmul_aa(*builder_, left_value, right_value));
}

auto ABY3Lower::unpack_cc(Value x,
                          Value y) -> std::pair<CipherValue, CipherValue> {
  assert(x.kind == ValueKind::kCipherValue);
  assert(y.kind == ValueKind::kCipherValue);
  auto x_value = cipher_values_[x.offset];
  auto y_value = cipher_values_[y.offset];
  return {x_value, y_value};
}

} // namespace fastmpc::flux::aby3