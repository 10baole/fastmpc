#include "fastmpc/flux/low/3pc/function/3pc_binary.h"
#include "fastmpc/flux/low/3pc/3pc_value.h"

namespace fastmpc::flux::_3pc {

    auto add_aa(FluxBuilder &builder, CipherValue x, CipherValue y) -> CipherValue {
        auto [p0_x0, p0_x1, p1_x1, p1_x2, p2_x2, p2_x0] = x;
        auto [p0_y0, p0_y1, p1_y1, p1_y2, p2_y2, p2_y0] = y;

        return CipherValue{
            .p0_v0 = builder.add(p0_x0, p0_y0),
            .p0_v1 = builder.add(p0_x1, p0_y1),

            .p1_v1 = builder.add(p1_x1, p1_y1),
            .p1_v2 = builder.add(p1_x2, p1_y2),

            .p2_v2 = builder.add(p2_x2, p2_y2),
            .p2_v0 = builder.add(p2_x0, p2_y0),
        };
    }

    auto add_ap(FluxBuilder &builder, CipherValue x, PlainValue y) -> CipherValue {
        auto [p0_x0, p0_x1, p1_x1, p1_x2, p2_x2, p2_x0] = x;
        auto [p0_y, p1_y, p2_y] = y;

        return CipherValue{
            .p0_v0 = builder.add(p0_x0, p0_y),
            .p0_v1 = p0_x1,

            .p1_v1 = builder.add(p1_x1, p1_y),
            .p1_v2 = p1_x2,

            .p2_v2 = builder.add(p2_x2, p2_y),
            .p2_v0 = p2_x0,
        };
    }

    auto add_pp(FluxBuilder &builder, PlainValue x, PlainValue y) -> PlainValue {
        auto [p0_x, p1_x, p2_x] = x;
        auto [p0_y, p1_y, p2_y] = y;

        return PlainValue{
            .p0_v = builder.add(p0_x, p0_y),
            .p1_v = builder.add(p1_x, p1_y),
            .p2_v = builder.add(p2_x, p2_y),
        };
    }

    auto multiply_aa(FluxBuilder &builder, CipherValue x, CipherValue y) -> CipherValue {
        auto [p0_x0, p0_x1, p1_x1, p1_x2, p2_x2, p2_x0] = x;
        auto [p0_y0, p0_y1, p1_y1, p1_y2, p2_y2, p2_y0] = y;

        return CipherValue{
            .p0_v0 = builder.multiply(p0_x0, p0_y0),
            .p0_v1 = builder.multiply(p0_x1, p0_y1),

            .p1_v1 = builder.multiply(p1_x1, p1_y1),
            .p1_v2 = builder.multiply(p1_x2, p1_y2),

            .p2_v2 = builder.multiply(p2_x2, p2_y2),
            .p2_v0 = builder.multiply(p2_x0, p2_y0),
        };
    }

    auto multiply_ap(FluxBuilder &builder, CipherValue x, PlainValue y) -> CipherValue {
        auto [p0_x0, p0_x1, p1_x1, p1_x2, p2_x2, p2_x0] = x;
        auto [p0_y, p1_y, p2_y] = y;

        return CipherValue{
            .p0_v0 = builder.multiply(p0_x0, p0_y),
            .p0_v1 = builder.multiply(p0_x1, p0_y),

            .p1_v1 = builder.multiply(p1_x1, p1_y),
            .p1_v2 = builder.multiply(p1_x2, p1_y),

            .p2_v2 = builder.multiply(p2_x2, p2_y),
            .p2_v0 = builder.multiply(p2_x0, p2_y),
        };
    }

    auto multiply_pp(FluxBuilder &builder, PlainValue x, PlainValue y) -> PlainValue {
        auto [p0_x, p1_x, p2_x] = x;
        auto [p0_y, p1_y, p2_y] = y;
        
        return PlainValue{
            .p0_v = builder.multiply(p0_x, p0_y),
            .p1_v = builder.multiply(p1_x, p1_y),
            .p2_v = builder.multiply(p2_x, p2_y),
        };
    }

}