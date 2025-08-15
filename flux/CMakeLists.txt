add_subdirectory(dialect)
add_subdirectory(low)
add_subdirectory(executor)

target_link_libraries(fastmpc
PUBLIC
  flux_dialect
)