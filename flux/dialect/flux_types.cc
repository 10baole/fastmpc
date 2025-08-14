#include "fastmpc/flux/dialect/flux_types.h"

#include <ostream>

#include "fastmpc/flux/dialect/flux_context.h"

namespace fastmpc::flux {

void Type::print(std::ostream &out, const FluxContext &context) const {
  out << holder << ".tensor<";
  context.shape(shape).print(out);
  out << '>';
}

} // namespace fastmpc::flux