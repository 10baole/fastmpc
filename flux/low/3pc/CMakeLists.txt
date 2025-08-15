add_subdirectory(function)

add_library(3pc
STATIC
    3pc_low.cc
    3pc_lower.cc  
)

target_link_libraries(3pc
PUBLIC
    abp_dialect
    3pc_function  
)