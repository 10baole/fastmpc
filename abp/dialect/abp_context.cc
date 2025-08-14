#include "fastmpc/abp/dialect/abp_context.h"

#include <functional>
#include <ostream>
#include <type_traits>

#include "fastmpc/abp/dialect/abp_ops.h"
#include "fastmpc/abp/dialect/abp_types.h"

namespace fastmpc::abp {

namespace {

template <class Op> auto type_of(Op &&op) -> Type { return op.type; }

template <> auto type_of<const OutputOp &>(const OutputOp &) -> Type {
  std::abort();
}

} // namespace

auto ABPContext::type(OpHandle handle) const -> Type {
  return visit(handle, [](OpHandle, auto &&op) { return type_of(op); });
}

void ABPContext::print(std::ostream &out, OpHandle handle) const {
  return visit(handle, [&](OpHandle, auto &&op) { op.print(out, *this); });
}

void ABPContext::print(std::ostream &out, ValuePrinter *printer) const {
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

} // namespace fastmpc::abp