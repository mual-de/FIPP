project(PipelineElements)
add_library(PipelineElements SHARED GenericSource.cpp GenericSink.cpp GenericSinkSi.cpp GenericPlugin.cpp GenericPluginSiSo.cpp GenericPluginSiSoWP.cpp InputFilter.cpp)
target_include_directories(PipelineElements PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../include>  
$<INSTALL_INTERFACE:include/FIPP/PipelineElements>)
find_package(yaml-cpp)
target_link_libraries(PipelineElements PUBLIC yaml-cpp Point ImageContainer Logging)
set(PipelineElementsHeaders
    "${PROJECT_SOURCE_DIR}/../../include/PipelineElements/GenericPlugin.hpp"
    "${PROJECT_SOURCE_DIR}/../../include/PipelineElements/GenericPluginSiSo.hpp"
    "${PROJECT_SOURCE_DIR}/../../include/PipelineElements/GenericPluginSiSoWP.hpp"
    "${PROJECT_SOURCE_DIR}/../../include/PipelineElements/GenericSink.hpp"
    "${PROJECT_SOURCE_DIR}/../../include/PipelineElements/GenericSinkSi.hpp"
    "${PROJECT_SOURCE_DIR}/../../include/PipelineElements/GenericSinkMi.hpp"
    "${PROJECT_SOURCE_DIR}/../../include/PipelineElements/GenericSource.hpp"
    "${PROJECT_SOURCE_DIR}/../../include/PipelineElements/IGenericPipelineElement.hpp"
    "${PROJECT_SOURCE_DIR}/../../include/PipelineElements/IGenericPlugin.hpp"
    "${PROJECT_SOURCE_DIR}/../../include/PipelineElements/IGenericSink.hpp"
    "${PROJECT_SOURCE_DIR}/../../include/PipelineElements/IGenericSource.hpp"
    "${PROJECT_SOURCE_DIR}/../../include/PipelineElements/InputFilter.hpp"
)
set_target_properties(PipelineElements PROPERTIES PUBLIC_HEADER "${PipelineElementsHeaders}")
install(TARGETS PipelineElements 
        EXPORT PipelineElementsTargets
        RUNTIME DESTINATION bin/FIPP
        LIBRARY DESTINATION lib/FIPP
        PUBLIC_HEADER DESTINATION include/FIPP/PipelineElements)

install(
                EXPORT PipelineElementsTargets 
                FILE PipelineElementsTargets.cmake
                NAMESPACE FIPP::
                DESTINATION "lib/cmake/FIPP/PipelineElements"
            )
include(CMakePackageConfigHelpers)

write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/PipelineElementsConfigVersion.cmake"
  VERSION "${PipelineElements_VERSION_MAJOR}.${PipelineElements_VERSION_MINOR}"
  COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/PipelineElementsConfig.cmake.in
            "${CMAKE_CURRENT_BINARY_DIR}/PipelineElementsConfig.cmake"
            INSTALL_DESTINATION "lib/cmake/FIPP/PipelineElements"
            NO_SET_AND_CHECK_MACRO
            NO_CHECK_REQUIRED_COMPONENTS_MACRO
            )


install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/PipelineElementsConfig.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/PipelineElementsConfigVersion.cmake
  DESTINATION lib/cmake/FIPP/PipelineElements
  )

  export(EXPORT PipelineElementsTargets
  FILE "${CMAKE_CURRENT_BINARY_DIR}/PipelineElementsTargets.cmake"
)