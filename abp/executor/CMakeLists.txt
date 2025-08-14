add_library(abp_executor
STATIC
  abp_executor.cc
)

target_link_libraries(abp_executor
PUBLIC
  abp_dialect
  eager
)

target_include_directories(abp_executor
PUBLIC
  ${FASTMPC_INCLUDE_DIRS}
)