#pragma once
#include "fastmpc/flux/dialect/flux_ops.h"
#include <array>

namespace fastmpc::flux::_3pc {

    enum class ValueKind {
        kPlainValue,
        kCipherValue,
    };

    struct PlainValue {
        OpHandle p0_v;
        OpHandle p1_v;
        OpHandle p2_v;
    };

    struct CipherValue {
        OpHandle p0_v0;
        OpHandle p0_v1;

        OpHandle p1_v1;
        OpHandle p1_v2;

        OpHandle p2_v2;
        OpHandle p2_v0;
    };

    inline auto cast(CipherValue in) {
        return _3pc::CipherValue{
            .p0_v0 = in.p0_v0,
            .p0_v1 = in.p0_v1,
            .p1_v1 = in.p1_v1,
            .p1_v2 = in.p1_v2,
            .p2_v2 = in.p2_v2,
            .p2_v0 = in.p2_v0,
        };
    }

    inline auto cast(PlainValue in) {
        return _3pc::PlainValue{
            .p0_v = in.p0_v,
            .p1_v = in.p1_v,
            .p2_v = in.p2_v,
        };
    }

    inline auto cast(_3pc::CipherValue in) {
        return CipherValue{
            .p0_v0 = in.p0_v0,
            .p0_v1 = in.p0_v1,
            .p1_v1 = in.p1_v1,
            .p1_v2 = in.p1_v2,
            .p2_v2 = in.p2_v2,
            .p2_v0 = in.p2_v0,
        };
    } 

    inline auto cast(_3pc::PlainValue in) {
        return PlainValue{
            .p0_v = in.p0_v,
            .p1_v = in.p1_v,
            .p2_v = in.p2_v,
        };
    }

    struct Value {
        ValueKind kind;
        size_t offset;
    };

}