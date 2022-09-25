add_executable(ImageContainerTest
    ImageContainer_test.cpp
)

target_link_libraries(ImageContainerTest
    PRIVATE
    GTest::GTest
    ImageContainer
    Point
)

add_test(
    NAME ImageContainerTest
    COMMAND ImageContainerTest
)