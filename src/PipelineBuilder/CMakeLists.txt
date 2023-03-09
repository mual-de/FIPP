project(PipelineBuilder)
add_library(PipelineBuilder SHARED PipelineBuilder.cpp PluginCreator.cpp)
find_package(yaml-cpp)
target_link_libraries(PipelineBuilder PUBLIC yaml-cpp PipelineElements Logging)
target_include_directories(PipelineBuilder PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../include>  
$<INSTALL_INTERFACE:include/FIPP/PipelineBuilder>)
set(PipelineBuilderHeaders
    "${PROJECT_SOURCE_DIR}/../../include/PipelineBuilder/PipelineBuilder.hpp"
    "${PROJECT_SOURCE_DIR}/../../include/PipelineBuilder/PluginCreator.hpp"
)
set_target_properties(PipelineBuilder PROPERTIES PUBLIC_HEADER "${PipelineBuilderHeaders}")
install(TARGETS PipelineBuilder 
            EXPORT PipelineBuilderTargets
            RUNTIME DESTINATION bin/FIPP
            LIBRARY DESTINATION lib/FIPP
            PUBLIC_HEADER DESTINATION include/FIPP/PipelineBuilder)

            install(
                EXPORT PipelineBuilderTargets 
                FILE PipelineBuilderTargets.cmake
                DESTINATION "lib/cmake/FIPP/PipelineBuilder"
            )
include(CMakePackageConfigHelpers)

configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/PipelineBuilderConfig.cmake.in
            "${CMAKE_CURRENT_BINARY_DIR}/PipelineBuilderConfig.cmake"
            INSTALL_DESTINATION "lib/cmake/FIPP/PipelineBuilder"
            NO_SET_AND_CHECK_MACRO
            NO_CHECK_REQUIRED_COMPONENTS_MACRO
            )
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/PipelineBuilderConfigVersion.cmake"
  VERSION "${PipelineBuilder_VERSION_MAJOR}.${PipelineBuilder_VERSION_MINOR}"
  COMPATIBILITY AnyNewerVersion
)

install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/PipelineBuilderConfig.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/PipelineBuilderConfigVersion.cmake
  DESTINATION lib/cmake/FIPP/PipelineBuilder
  )

  export(EXPORT PipelineBuilderTargets
  FILE "${CMAKE_CURRENT_BINARY_DIR}/PipelineBuilderTargets.cmake"
)