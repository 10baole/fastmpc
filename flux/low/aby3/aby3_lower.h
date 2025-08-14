#pragma once

#include <map>

#include "fastmpc/abp/dialect/abp_context.h"
#include "fastmpc/abp/dialect/abp_ops.h"
#include "fastmpc/flux/dialect/flux_builder.h"
#include "fastmpc/flux/low/3pc/3pc_lower.h"
#include "fastmpc/flux/low/aby3/aby3_value.h"

namespace fastmpc::flux::aby3 {

class ABY3Lower final : public _3pc::_3PCLower {
public:
  ABY3Lower(FluxBuilder &builder, const abp::ABPContext &abp_context)
      : _3PCLower(builder, abp_context) {}
  void run();
  using _3pc::_3PCLower::operator();
  void operator()(abp::OpHandle handle, abp::TruncateAOp op);
  void operator()(abp::OpHandle handle, abp::A2BOp op);
  void operator()(abp::OpHandle handle, abp::B2AOp op);
  void operator()(abp::OpHandle handle, abp::AddAAOp op);
  void operator()(abp::OpHandle handle, abp::MultiplyAAOp op);
  void operator()(abp::OpHandle handle, abp::XorBBOp op);
  void operator()(abp::OpHandle handle, abp::AndBBOp op);
  void operator()(abp::OpHandle handle, abp::DotGeneralAAOp op);

private:
  auto get_cipher_value(abp::OpHandle handle) -> _3pc::CipherValue override;
  auto get_plain_value(abp::OpHandle handle) -> _3pc::PlainValue override;
  void visit_value(abp::OpHandle handle, _3pc::ValueVisitor &visitor) override;
  void set_value(abp::OpHandle handle, _3pc::CipherValue value) override;
  void set_value(abp::OpHandle handle, _3pc::PlainValue value) override;

  void push(abp::OpHandle handle, PlainValue value);
  void push(abp::OpHandle handle, CipherValue value);

  auto unpack_cc(Value x, Value y) -> std::pair<CipherValue, CipherValue>;

  std::vector<PlainValue> plain_values_;
  std::vector<CipherValue> cipher_values_;

  std::map<abp::OpHandle, Value> map_;
};

} // namespace fastmpc::flux::aby3