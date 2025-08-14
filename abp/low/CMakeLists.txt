add_library(abp_low
STATIC
  abp_lower.cc
  abp_low.cc
)

target_link_libraries(abp_low
PRIVATE
  abp_function
  pphlo_dialect
)

target_include_directories(abp_low
PUBLIC
  ${FASTMPC_INCLUDE_DIRS}
)