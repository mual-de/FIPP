add_executable(PointTest
    Point_test.cpp
)

find_package(yaml-cpp)
target_link_libraries(PointTest
    PRIVATE
    GTest::GTest
    Point
    yaml-cpp
)

add_test(
    NAME PointTest
    COMMAND PointTest
)