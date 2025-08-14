#include "fastmpc/flux/low/aby3/aby3_low.h"
#include "fastmpc/flux/dialect/flux_builder.h"
#include "fastmpc/flux/dialect/flux_context.h"
#include "fastmpc/flux/low/aby3/aby3_lower.h"

namespace fastmpc::flux::aby3 {

auto low(const abp::ABPContext &context) -> FluxContext {
  FluxContext result;
  FluxBuilder builder(result);
  ABY3Lower lower(builder, context);
  lower.run();
  return result;
}

}