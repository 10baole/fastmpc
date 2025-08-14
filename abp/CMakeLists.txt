add_subdirectory(dialect)
add_subdirectory(executor)
add_subdirectory(function)
add_subdirectory(low)

target_link_libraries(fastmpc
PUBLIC
  abp_dialect
  abp_low
)