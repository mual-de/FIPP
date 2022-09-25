add_executable(PointTest
    Point_test.cpp
)

target_link_libraries(PointTest
    PRIVATE
    GTest::GTest
    Point
)

add_test(
    NAME PointTest
    COMMAND PointTest
)