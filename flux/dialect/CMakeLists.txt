add_library(flux_dialect
STATIC
  flux_builder.cc
  flux_context.cc
  flux_ops.cc
  flux_types.cc
)

target_link_libraries(flux_dialect
PUBLIC
  ir_base
)

target_include_directories(flux_dialect
PUBLIC
  ${FASTMPC_INCLUDE_DIRS}
)