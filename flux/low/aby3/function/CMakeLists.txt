add_library(aby3_function
STATIC
  aby3_binary.cc
  aby3_casting.cc
  aby3_unary.cc
)

target_include_directories(aby3_function
PUBLIC
  ${FASTMPC_INCLUDE_DIRS}
)

target_link_libraries(aby3_function
PUBLIC
  flux_dialect
  3pc_function
)

add_executable(aby3_function_test
  aby3_function_test.cc
)

target_link_libraries(aby3_function_test
PUBLIC
  aby3_function
  flux_executor
  gtest
  gtest_main
)