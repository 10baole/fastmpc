#pragma once

#include <cstddef>
#include <cstdint>
#include <functional>
#include <iosfwd>
#include <vector>

#include "fastmpc/abp/dialect/abp_types.h"

namespace fastmpc::abp {

enum class OpKind {
  kInputOp,
  kOutputOp,
  kConstantOp,

  // unary ops
  kNegateAOp,
  kNegatePOp,
  kInverseOp,
  kTruncateAOp,
  kTruncatePOp,
  kNotBOp,
  kBitReverseOp,
  kShiftRightOp,
  kP2AOp,
  kA2BOp,
  kB2AOp,
  kBroadcastOp,
  kReshapeOp,
  kSliceOp,
  kTransposeOp,

  // binary ops
  kAddAAOp,
  kAddAPOp,
  kAddPPOp,
  kMultiplyAAOp,
  kMultiplyAPOp,
  kMultiplyPPOp,
  kXorBBOp,
  kAndBBOp,
  kDotGeneralAAOp,

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

struct Op {
  using handle_type = OpHandle;
  OpKind kind;
  size_t offset;
  
  auto hash() const -> size_t;
  auto operator==(const Op &other) const -> bool {
    return kind == other.kind && offset == other.offset;
  }
};

struct InputOp {
  static constexpr OpKind kind = OpKind::kInputOp;
  using handle_type = size_t;
  Type type;
  size_t input_index;

  void print(std::ostream &out, const ABPContext &context) const;
};

struct OutputOp {
  static constexpr OpKind kind = OpKind ::kOutputOp;
  using handle_type = size_t;
  OpHandle operand;
  size_t output_index;

  void print(std ::ostream &out, const ABPContext &context) const;
};

struct ConstantOp {
  static constexpr OpKind kind = OpKind ::kConstantOp;
  using handle_type = size_t;
  Type type;
  DenseValueHandle value;

  auto hash() const -> size_t;
  void print(std::ostream &out, const ABPContext &context) const;
  auto operator==(const ConstantOp &other) const -> bool;
};

#define DECL_UNARY_OP(OpName, ...)                                             \
  struct OpName {                                                              \
    static constexpr OpKind kind = OpKind::k##OpName;                          \
    using handle_type = size_t;                                                \
    Type type;                                                                 \
    OpHandle operand;                                                          \
    __VA_ARGS__                                                                \
    auto hash() const -> size_t;                                               \
    void print(std::ostream &out, const ABPContext &context) const;            \
    auto operator==(const OpName &other) const -> bool;                        \
  }
DECL_UNARY_OP(NegateAOp);
DECL_UNARY_OP(NegatePOp);
DECL_UNARY_OP(InverseOp);
DECL_UNARY_OP(NotBOp);
DECL_UNARY_OP(BitReverseOp);
DECL_UNARY_OP(ShiftRightOp, uint8_t bits;);
DECL_UNARY_OP(TruncateAOp, uint8_t bits;);
DECL_UNARY_OP(TruncatePOp, uint8_t bits;);
DECL_UNARY_OP(P2AOp);
DECL_UNARY_OP(A2BOp);
DECL_UNARY_OP(B2AOp);
DECL_UNARY_OP(BroadcastOp, DenseSizeTHandle dimensions;);
DECL_UNARY_OP(ReshapeOp);
DECL_UNARY_OP(SliceOp, DenseSizeTHandle start; DenseSizeTHandle end;
              DenseSizeTHandle stride;);
DECL_UNARY_OP(TransposeOp, DenseSizeTHandle permutation;);
#undef DECL_UNARY_OP

#define DECL_BINARY_OP(OpName, ...)                                            \
  struct OpName {                                                              \
    static constexpr OpKind kind = OpKind::k##OpName;                          \
    using handle_type = size_t;                                                \
    Type type;                                                                 \
    OpHandle left;                                                             \
    OpHandle right;                                                            \
    __VA_ARGS__                                                                \
    auto hash() const -> size_t;                                               \
    void print(std::ostream &out, const ABPContext &context) const;            \
    auto operator==(const OpName &other) const -> bool;                        \
  }
DECL_BINARY_OP(AddAAOp);
DECL_BINARY_OP(AddAPOp);
DECL_BINARY_OP(AddPPOp);
DECL_BINARY_OP(MultiplyAAOp);
DECL_BINARY_OP(MultiplyAPOp);
DECL_BINARY_OP(MultiplyPPOp);
DECL_BINARY_OP(XorBBOp);
DECL_BINARY_OP(AndBBOp);
DECL_BINARY_OP(DotGeneralAAOp);
#undef DECL_BINARY_OP

struct ConcateOp {
  static constexpr OpKind kind = OpKind::kConcateOp;
  using handle_type = size_t;
  Type type;
  std::vector<OpHandle> operands;
  size_t dimension;

  auto hash() const -> size_t;
  void print(std::ostream &out, const ABPContext &context) const;
  auto operator==(const ConcateOp &other) const -> bool;
};

} // namespace fastmpc::abp