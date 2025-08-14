#pragma once

#include "fastmpc/flux/dialect/flux_types.h"
#include "fastmpc/ir_base/attribute.h"

namespace fastmpc::flux {

class FluxContext;

enum class OpKind {
  kInputOp,
  kOutputOp,

  kAShiftRightOp,
  kBitReverseOp,
  kBroadcastOp,
  kCastOp,
  kInverseOp,
  kLShiftRightOp,
  kNegateOp,
  kNotOp,
  kReshapeOp,
  kShiftLeftOp,
  kSliceOp,
  kTransposeOp,

  kAddOp,
  kAndOp,
  kMatmulOp,
  kMultiplyOp,
  kSubtractOp,
  kXorOp,

  kConstantOp,
  kRandomOp,

  kConcateOp,
};

class OpHandle {
public:
  explicit OpHandle(size_t handle) : handle_(handle) {}
  void print(std::ostream &out) const;
  auto unwarp() const -> size_t { return handle_; }
  auto operator==(const OpHandle &other) const -> bool {
    return handle_ == other.handle_;
  }
  auto operator<(const OpHandle &other) const -> bool {
    return handle_ < other.handle_;
  }

private:
  size_t handle_;
};

struct InputOp {
  static constexpr OpKind kind = OpKind::kInputOp;
  using handle_type = size_t;
  Type type;
  size_t input_index;
  size_t tuple_index;

  void print(std::ostream &out, const FluxContext &) const;
};

struct OutputOp {
  static constexpr OpKind kind = OpKind::kOutputOp;
  using handle_type = size_t;
  OpHandle operand;
  size_t output_index;
  size_t tuple_index;

  void print(std::ostream &out, const FluxContext &) const;
};

#define DECL_BINARY_OP(OpName, ...)                                            \
  struct OpName {                                                              \
    static constexpr OpKind kind = OpKind::k##OpName;                          \
    using handle_type = size_t;                                                \
    Type type;                                                                 \
    OpHandle left;                                                             \
    OpHandle right;                                                            \
    __VA_ARGS__                                                                \
    void print(std::ostream &out, const FluxContext &) const;                  \
  }

DECL_BINARY_OP(AddOp);
DECL_BINARY_OP(AndOp);
DECL_BINARY_OP(MatmulOp);
DECL_BINARY_OP(MultiplyOp);
DECL_BINARY_OP(SubtractOp);
DECL_BINARY_OP(XorOp);
#undef DECL_BINARY_OP

#define DECL_UNARY_OP(OpName, ...)                                             \
  struct OpName {                                                              \
    static constexpr OpKind kind = OpKind::k##OpName;                          \
    using handle_type = size_t;                                                \
    Type type;                                                                 \
    OpHandle operand;                                                          \
    __VA_ARGS__                                                                \
    void print(std::ostream &out, const FluxContext &) const;                  \
  }

DECL_UNARY_OP(AShiftRightOp, uint8_t bits;);
DECL_UNARY_OP(BitReverseOp);
DECL_UNARY_OP(BroadcastOp, DenseSizeTHandle dimensions;);
DECL_UNARY_OP(CastOp);
DECL_UNARY_OP(InverseOp, uint8_t fixed_point;);
DECL_UNARY_OP(LShiftRightOp, uint8_t bits;);
DECL_UNARY_OP(NegateOp);
DECL_UNARY_OP(NotOp);
DECL_UNARY_OP(ReshapeOp);
DECL_UNARY_OP(ShiftLeftOp, uint8_t bits;);
DECL_UNARY_OP(SliceOp, DenseSizeTHandle start; DenseSizeTHandle end;
              DenseSizeTHandle stride;);
DECL_UNARY_OP(TransposeOp, DenseSizeTHandle permutation;);
#undef DECL_UNARY_OP

struct RandomOp {
  static constexpr OpKind kind = OpKind::kRandomOp;
  using handle_type = size_t;
  Type type;
  size_t rng_index;
  size_t rng_seed;

  void print(std::ostream &out, const FluxContext &) const;
};

struct ConstantOp {
  static constexpr OpKind kind = OpKind::kConstantOp;
  using handle_type = size_t;
  Type type;
  DenseValueHandle value;

  void print(std::ostream &out, const FluxContext &) const;
};

struct ConcateOp {
  static constexpr OpKind kind = OpKind::kConcateOp;
  using handle_type = size_t;
  Type type;
  std::vector<OpHandle> operands;
  size_t dimension;

  void print(std::ostream &out, const FluxContext &) const;
};

struct Op {
  using handle_type = OpHandle;
  OpKind kind;
  size_t offset;
};

} // namespace fastmpc::flux