#include "fastmpc/abp/low/abp_low.h"


#include "fastmpc/abp/dialect/abp_builder.h"
#include "fastmpc/abp/low/abp_lower.h"

namespace fastmpc::abp {

auto low(const pphlo::PPHLOContext &context) -> ABPContext {
  ABPContext result;
  ABPBuilder builder(result, 15);
  ABPLower lower(context, builder);
  lower.run();
  return result;
}

}