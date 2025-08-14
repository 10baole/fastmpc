#include "fastmpc/flux/low/3pc/function/3pc_io.h"
#include "fastmpc/flux/dialect/flux_ops.h"
#include "fastmpc/flux/low/3pc/3pc_value.h"

namespace fastmpc::flux::_3pc {

template <>
auto input<PlainValue>(FluxBuilder &builder, size_t input_index,
                       ShapeHandle shape) -> PlainValue {
  enum : size_t {
    P0 = 0,
    P1 = 1,
    P2 = 2,
  };
  auto input = [&](size_t holder) {
    Type type{holder, shape};
    return builder.input(input_index, 0, type);
  };
  return PlainValue{
      .p0_v = input(P0),
      .p1_v = input(P1),
      .p2_v = input(P2),
  };
}

namespace {
enum : size_t {
  P0 = 0,
  P1 = 1,
  P2 = 2,
  T0 = 0,
  T1 = 1,
  T2 = 2,
};
}

template <>
auto input<CipherValue>(FluxBuilder &builder, size_t input_index,
                        ShapeHandle shape) -> CipherValue {
  auto input = [&](size_t holder, size_t tuple_index) {
    Type type{holder, shape};
    return builder.input(input_index, tuple_index, type);
  };
  return CipherValue{
      .p0_v0 = input(P0, T0),
      .p0_v1 = input(P0, T1),
      .p1_v1 = input(P1, T1),
      .p1_v2 = input(P1, T2),
      .p2_v2 = input(P2, T2),
      .p2_v0 = input(P2, T0),
  };
}

void output(FluxBuilder &builder, size_t output_index, PlainValue x) {
  auto output = [&](OpHandle handle) {
    return builder.output(handle, output_index, 0);
  };
  output(x.p0_v);
  output(x.p1_v);
  output(x.p2_v);
}

void output(FluxBuilder &builder, size_t output_index, CipherValue value) {
  auto output = [&](OpHandle handle, size_t tuple_index) {
    return builder.output(handle, output_index, tuple_index);
  };
  output(value.p0_v0, T0);
  output(value.p0_v1, T1);
  output(value.p1_v1, T1);
  output(value.p1_v2, T2);
  output(value.p2_v2, T2);
  output(value.p2_v0, T0);
}

} // namespace fastmpc::flux::_3pc