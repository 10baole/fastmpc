#include "fastmpc/flux/low/3pc/3pc_low.h"
#include "fastmpc/flux/dialect/flux_builder.h"
#include "fastmpc/flux/dialect/flux_context.h"
#include "fastmpc/flux/low/3pc/3pc_lower.h"

namespace fastmpc::flux::_3pc {

    auto low(const abp::ABPContext &context) -> FluxContext {
        FluxContext result;
        FluxBuilder builder(result);
        _3PCLower lower(builder, context);
        lower.run();
        return result;
    }
    
}