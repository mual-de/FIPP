find_package(GTest REQUIRED)

add_subdirectory(Point)

function(do_test target result)
  add_test(NAME ${target}_test COMMAND ${target})
  set_tests_properties(${target}_test
    PROPERTIES PASS_REGULAR_EXPRESSION ${result}
  )
endfunction()