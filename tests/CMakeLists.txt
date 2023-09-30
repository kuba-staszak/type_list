add_executable (test1 main.cc)
target_link_libraries(test1 PRIVATE type_list)

add_test(
  NAME test1
  COMMAND $<TARGET_FILE:test1>
)
