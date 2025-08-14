#pragma once

#include <array>

#include "fastmpc/flux/dialect/flux_ops.h"
#include "fastmpc/flux/low/3pc/3pc_value.h"

namespace fastmpc::flux::aby3 {

enum class ValueKind {
  kPlainValue,
  kCipherValue,
};

struct PlainValue {
  OpHandle p0_x;
  OpHandle p1_x;
  OpHandle p2_x;
};

struct CipherValue {
  OpHandle p0_x0;
  OpHandle p0_x1;

  OpHandle p1_x1;
  OpHandle p1_x2;

  OpHandle p2_x2;
  OpHandle p2_x0;
};

inline auto cast(CipherValue in) {
  return _3pc::CipherValue{
      .p0_v0 = in.p0_x0,
      .p0_v1 = in.p0_x1,
      .p1_v1 = in.p1_x1,
      .p1_v2 = in.p1_x2,
      .p2_v2 = in.p2_x2,
      .p2_v0 = in.p2_x0,
  };
}

inline auto cast(PlainValue in) {
  return _3pc::PlainValue{
      .p0_v = in.p0_x,
      .p1_v = in.p1_x,
      .p2_v = in.p2_x,
  };
}

inline auto cast(_3pc::CipherValue in) {
  return CipherValue{
      .p0_x0 = in.p0_v0,
      .p0_x1 = in.p0_v1,
      .p1_x1 = in.p1_v1,
      .p1_x2 = in.p1_v2,
      .p2_x2 = in.p2_v2,
      .p2_x0 = in.p2_v0,
  };
}

inline auto cast(_3pc::PlainValue in) {
  return PlainValue{
      .p0_x = in.p0_v,
      .p1_x = in.p1_v,
      .p2_x = in.p2_v,
  };
}

struct Value {
  ValueKind kind;
  size_t offset;
};

} // namespace fastmpc::flux::aby3