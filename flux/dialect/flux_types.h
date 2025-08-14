#pragma once

#include "fastmpc/ir_base/attribute.h"

namespace fastmpc::flux {

class FluxContext;

struct Type {
  size_t holder;
  ShapeHandle shape;

  void print(std::ostream &out, const FluxContext &) const;
  auto operator==(const Type &other) const -> bool {
    return holder == other.holder && shape == other.shape;
  }
};

} // namespace fastmpc::flux