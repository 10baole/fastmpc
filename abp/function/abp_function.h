// #pragma once

// #include "fastmpc/abp/dialect/abp_builder.h"
// #include "fastmpc/abp/dialect/abp_ops.h"

// namespace fastmpc::abp {
// auto add(ABPBuilder &builder, OpHandle left, OpHandle right) -> OpHandle;
// auto subtract(ABPBuilder &builder, OpHandle left, OpHandle right) ->
// OpHandle; auto multiply(ABPBuilder &builder, OpHandle left, OpHandle right)
// -> OpHandle; auto dot_general(ABPBuilder &builder, OpHandle left, OpHandle
// right)
//     -> OpHandle;
// auto bitwise_and(ABPBuilder &builder, OpHandle left, OpHandle right)
//     -> OpHandle;
// auto bitwise_xor(ABPBuilder &builder, OpHandle left, OpHandle right)
//     -> OpHandle;
// auto bitwise_or(ABPBuilder &builder, OpHandle left, OpHandle right) ->
// OpHandle; auto divide(ABPBuilder &builder, OpHandle left, OpHandle right) ->
// OpHandle;

// auto negate(ABPBuilder &builder, OpHandle operand) -> OpHandle;
// auto truncate(ABPBuilder &builder, OpHandle operand, uint8_t bits) ->
// OpHandle;

// auto a2b(ABPBuilder &builder, OpHandle operand) -> OpHandle;
// auto b2a(ABPBuilder &builder, OpHandle operand, uint8_t fixed_point)
//     -> OpHandle;

// auto constant(ABPBuilder &builder, float value) -> OpHandle;
// auto constant(ABPBuilder &builder, uint64_t value) -> OpHandle;
// auto constant(ABPBuilder &builder, std::vector<float> &&value, Shape &&shape)
//     -> OpHandle;
// auto constant(ABPBuilder &builder, std::vector<uint64_t> &&value, Shape
// &&shape)
//     -> OpHandle;
// auto exp(ABPBuilder &builder, size_t x) -> OpHandle;

// // compare.cc
// auto equal(ABPBuilder &builder, OpHandle left, OpHandle right) -> OpHandle;
// auto less(ABPBuilder &builder, OpHandle left, OpHandle right) -> OpHandle;
// auto greater(ABPBuilder &builder, OpHandle left, OpHandle right) -> OpHandle;
// auto greater_equal(ABPBuilder &builder, OpHandle left, OpHandle right)
//     -> OpHandle;

// auto select(ABPBuilder &builder, OpHandle which, OpHandle left, OpHandle
// right)
//     -> OpHandle;
// auto max(ABPBuilder &builder, OpHandle left, OpHandle right) -> OpHandle;
// auto log2(ABPBuilder &builder, OpHandle x) -> OpHandle;
// auto log(ABPBuilder &builder, OpHandle x) -> OpHandle;

// // reduce.cc
// using ReduceFunc = auto (*)(ABPBuilder &, OpHandle, OpHandle) -> OpHandle;
// auto reduce(ABPBuilder &builder, OpHandle x, OpHandle init, DenseSizeT
// &&dims,
//             DenseSizeT &&shape, ReduceFunc reducer) -> size_t;

// namespace unsafe {
// // auto constant(ABPBuilder &builder, uint64_t value, uint8_t fixed_point,
// //               Shape &&shape) -> size_t;
// auto iota(ABPBuilder &builder, size_t iota_dim, uint8_t fixed_point,
//           Shape &&shape) -> size_t;
// } // namespace unsafe
// } // namespace fastmpc::abp

#pragma once
#include "fastmpc/abp/function/abp_binary.h"
#include "fastmpc/abp/function/abp_compare.h"
#include "fastmpc/abp/function/abp_constant.h"
#include "fastmpc/abp/function/abp_divide.h"
#include "fastmpc/abp/function/abp_log2.h"
#include "fastmpc/abp/function/abp_reduce.h"
#include "fastmpc/abp/function/abp_unary.h"