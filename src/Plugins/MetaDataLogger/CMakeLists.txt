add_library(MetaDataLogger metaDataLogger.cpp)
find_package(yaml-cpp)
target_include_directories(MetaDataLogger PUBLIC "${PROJECT_SOURCE_DIR}/include")
target_link_libraries(MetaDataLogger PRIVATE yaml-cpp)