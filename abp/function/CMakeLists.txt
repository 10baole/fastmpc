add_library(abp_function
STATIC
    abp_binary.cc
    abp_circuit.cc
    abp_compare.cc
    abp_constant.cc
    abp_divide.cc
    abp_log2.cc
    abp_nn.cc
    abp_reduce.cc
    abp_unary.cc
)

target_link_libraries(abp_function
PUBLIC
    abp_dialect
)

target_include_directories(abp_function
PUBLIC
    ${FASTMPC_INCLUDE_DIRS}
)

add_executable(abp_function_test
    abp_function_test.cc
)

target_link_libraries(abp_function_test
PUBLIC
    abp_function
    abp_executor
    gtest
    gtest_main
)