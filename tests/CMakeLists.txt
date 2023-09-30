

add_executable (check_headers
    check_headers.cc
    check_header_type_list.cc
    check_header_type_list_impl.cc
    check_header_type_list_hdr.cc
    check_header_type_list_utilities.cc
)
target_link_libraries(check_headers PRIVATE type_list)

add_executable (test1 main.cc)
target_link_libraries(test1 PRIVATE type_list)

add_test(
  NAME test1
  COMMAND $<TARGET_FILE:test1>
)
