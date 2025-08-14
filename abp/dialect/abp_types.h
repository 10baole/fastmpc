#pragma once

#include "fastmpc/ir_base/attribute.h"

namespace fastmpc::abp {

enum class TypeKind : int8_t {
  kArithFixed64,
  kBitArray64,
  kFixed64,
};

struct Type {
  TypeKind kind;
  uint8_t fixed_point;
  ShapeHandle shape;

  auto hash() const -> size_t;
  void print(std::ostream &out, const class ABPContext &context) const;
  auto operator==(const Type &other) const -> bool;
};

constexpr auto encode(TypeKind left, TypeKind right) -> size_t {
  return static_cast<size_t>(left) * 3 + static_cast<size_t>(right);
}

} // namespace fastmpc::abp