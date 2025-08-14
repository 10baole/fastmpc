add_library(abp_dialect
STATIC
  abp_builder.cc
  abp_context.cc
  abp_ops.cc
  abp_types.cc
)

target_link_libraries(abp_dialect
PUBLIC
  ir_base
)

target_include_directories(abp_dialect
PUBLIC
  ${FASTMPC_INCLUDE_DIRS}
)