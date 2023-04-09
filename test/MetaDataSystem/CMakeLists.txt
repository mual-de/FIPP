add_executable(MetaDataSystemTest
    MetaDataSystem_test.cpp
)

target_link_libraries(MetaDataSystemTest
    PRIVATE
    GTest::GTest
    ImageContainer
    )
add_test(
    NAME MetaDataSystemTest
    COMMAND MetaDataSystemTest
)