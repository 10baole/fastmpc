add_library(flux_executor
  flux_executor.cc
)

target_link_libraries(flux_executor
PUBLIC
  eager
)

target_include_directories(flux_executor
PUBLIC
  ${FASTMPC_INCLUDE_DIRS}
)