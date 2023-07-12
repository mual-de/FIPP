add_subdirectory(PipelineElements)
add_subdirectory(Logging)
add_subdirectory(ImageContainer)
add_subdirectory(PipelineBuilder)
add_subdirectory(FIPP)

project(Point)
add_library(Point INTERFACE)
target_compile_features(Point INTERFACE cxx_std_17)

target_include_directories(Point INTERFACE 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../include>  
        $<INSTALL_INTERFACE:include/FIPP>)
set_target_properties(Point PROPERTIES PUBLIC_HEADER "${PROJECT_SOURCE_DIR}/../include/Point.hpp")
install(TARGETS Point 
        EXPORT PointTargets
        PUBLIC_HEADER DESTINATION include/FIPP/)
include(CMakePackageConfigHelpers)

write_basic_package_version_file("PointConfigVersion.cmake"
                                         VERSION 1.0.0
                                         COMPATIBILITY SameMajorVersion)
        