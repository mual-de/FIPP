cmake_minimum_required(VERSION 3.14)
project(FIPP)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(ENABLE_CUDA ON)

set(CMAKE_MODULE_PATH "${CMAKE_BINARY_DIR}" "${CMAKE_MODULE_PATH}")
set(CMAKE_PREFIX_PATH "${CMAKE_BINARY_DIR}" "${CMAKE_PREFIX_PATH}")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin")


option(BUILD_TESTS "Build tests" ON)

enable_testing()

add_subdirectory(src)
add_library(FIPP SHARED src/CentralController.cpp src/PipelineBuilder.cpp)
find_package(yaml-cpp)
target_link_libraries(FIPP PUBLIC Point Logging ImageContainer PipelineElements yaml-cpp)
set_target_properties(FIPP PROPERTIES PUBLIC_HEADER include/**)

if (BUILD_TESTS)
    add_subdirectory(test)
endif ()

