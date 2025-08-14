#include "fastmpc/flux/dialect/flux_context.h"

#include <cstdlib>
#include <ostream>

#include "fastmpc/flux/dialect/flux_ops.h"

namespace fastmpc::flux {

namespace {

template <class Op> auto type_of(Op &&op) -> Type { return op.type; }

template <> auto type_of<const OutputOp &>(const OutputOp &) -> Type {
  std::abort();
}

} // namespace

auto FluxContext::type(OpHandle handle) const -> Type {
  return visit(handle, [](OpHandle, auto &&op) { return type_of(op); });
}

void FluxContext::print(std::ostream &out, OpHandle handle) const {
  visit(handle, [&](OpHandle, auto &&op) { op.print(out, *this); });
}

void FluxContext::print(std::ostream &out, ValuePrinter *printer) const {
  for (size_t i = 0; i < ops_.size(); i++) {
    OpHandle handle(i);
    handle.print(out);
    out << " = ";
    print(out, handle);
    out << '\n';
    if (printer) {
      printer->print_value(out, handle);
      out << '\n';
    }
  }
}

} // namespace fastmpc::flux