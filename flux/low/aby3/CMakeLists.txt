add_subdirectory(function)

add_library(aby3_low
  aby3_lower.cc
  aby3_low.cc
)

target_link_libraries(aby3_low
PUBLIC
  3pc
  aby3_function
)