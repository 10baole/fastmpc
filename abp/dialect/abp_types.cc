#include "fastmpc/abp/dialect/abp_types.h"

#include <cstdint>
#include <ostream>

#include "fastmpc/abp/dialect/abp_context.h"
#include "fastmpc/ir_base/fvn_context.h"

namespace fastmpc::abp {

auto Type::hash() const -> size_t {
  FVNContext context;
  context.push(static_cast<uint64_t>(kind));
  context.push(static_cast<uint64_t>(fixed_point));
  context.push(static_cast<uint64_t>(shape.unwarp()));
  return static_cast<size_t>(context.value());
}

void Type::print(std::ostream &out, const ABPContext &context) const {
  out << "tensor<";
  context.shape(shape).print(out);
  switch (kind) {
  case TypeKind::kArithFixed64:
    out << '[' << static_cast<size_t>(fixed_point) << "]a64>";
    return;
  case TypeKind::kBitArray64:
    out << "b64>";
    return;
  case TypeKind::kFixed64:
    out << '[' << static_cast<size_t>(fixed_point) << "]p64>";
    return;
  }
}

auto Type::operator==(const Type &other) const -> bool {
  return kind == other.kind && fixed_point == other.fixed_point &&
         shape == other.shape;
}

} // namespace fastmpc::abp