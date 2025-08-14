#include "fastmpc/abp/function/abp_constant.h"
#include <numeric>

namespace fastmpc::abp {

auto constant(ABPBuilder &builder, float value) -> OpHandle {
  return constant(builder, std::vector<float>{value}, {});
}

auto constant(ABPBuilder &builder, uint64_t value) -> OpHandle {
  return constant(builder, std::vector<uint64_t>{value}, {});
}

namespace {

auto encode(std::vector<float> &&in, uint8_t fixed_point) {
  DenseValue out(in.size());
  for (size_t i = 0; i < in.size(); i++) {
    out[i] = static_cast<uint64_t>(
        static_cast<int64_t>(in[i] * (1ll << fixed_point)));
  }
  return out;
}

} // namespace

auto constant(ABPBuilder &builder, std::vector<float> &&value, Shape &&shape)
    -> OpHandle {
  uint8_t fixed_point = builder.fixed_point();
  Type type{
      .kind = TypeKind::kFixed64,
      .fixed_point = fixed_point,
      .shape = builder.push(std::move(shape)),
  };
  auto dense_value = encode(std::move(value), fixed_point);
  auto handle = builder.push(std::move(dense_value));
  return builder.constant(handle, type);
}

auto constant(ABPBuilder &builder, std::vector<uint64_t> &&value, Shape &&shape)
    -> OpHandle {
  Type type{
      .kind = TypeKind::kFixed64,
      .fixed_point = 0,
      .shape = builder.push(std::move(shape)),
  };
  DenseValue dense_value;
  dense_value.swap(value);
  auto handle = builder.push(std::move(dense_value));
  return builder.constant(handle, type);
}

auto iota(ABPBuilder &builder, size_t iota_dimension, Shape &&shape)
    -> OpHandle {
  size_t n = shape[iota_dimension];
  std::vector<uint64_t> data(n);
  std::iota(data.begin(), data.end(), 0);

  auto result = constant(builder, std::move(data), {n});
  if (shape.size() != 1) {
    auto handle = builder.push(std::move(shape));
    result = builder.broadcast(result, {iota_dimension}, handle);
  }
  return result;
}

} // namespace fastmpc::abp