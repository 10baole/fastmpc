add_library(3pc_function
STATIC
  3pc_binary.cc
  3pc_io.cc
  3pc_unary.cc
)

target_link_libraries(3pc_function
PUBLIC
  flux_dialect
)

target_include_directories(3pc_function
PUBLIC
  ${FASTMPC_INCLUDE_DIRS}
)

add_executable(3pc_function_test 
  3pc_function_test.cc
)

target_link_libraries(3pc_function_test
PUBLIC
  3pc_function
  flux_executor
  gtest
  gtest_main
)