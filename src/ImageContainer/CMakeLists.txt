project(ImageContainer VERSION 0.1.0)
set (CMAKE_SHARED_LINKER_FLAGS "-Wl,--no-as-needed")
if(ENABLE_CUDA)
add_library(ImageContainer SHARED ImageContainer.cpp ImageContainerCPU.cpp ImageContainerCUDA.cpp ImagePool.cpp ImagePoolFactory.cpp MetaDataSystem.cpp ImageFormat.cpp)
else()
add_library(ImageContainer SHARED ImageContainer.cpp ImageContainerCPU.cpp ImageContainerCUDA.cpp ImagePool.cpp ImagePoolFactory.cpp MetaDataSystem.cpp ImageFormat.cpp)
endif()
find_package(yaml-cpp)
find_package(CUDAToolkit REQUIRED)
target_include_directories(ImageContainer PUBLIC 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../include>  
        $<INSTALL_INTERFACE:include/FIPP/ImageContainer>)
target_link_libraries(ImageContainer PUBLIC yaml-cpp CUDA::cudart CUDA::cuda_driver)
set(ImageContainerHeaders
    "${PROJECT_SOURCE_DIR}/../../include/ImageContainer/IImageContainer.hpp"
    "${PROJECT_SOURCE_DIR}/../../include/ImageContainer/IImageContainerCUDA.hpp"
    "${PROJECT_SOURCE_DIR}/../../include/ImageContainer/ImageFormat.hpp"
    "${PROJECT_SOURCE_DIR}/../../include/ImageContainer/IImagePool.hpp"
    "${PROJECT_SOURCE_DIR}/../../include/ImageContainer/ImagePoolFactory.hpp"
    "${PROJECT_SOURCE_DIR}/../../include/ImageContainer/MetaDataSystem.hpp"
)
set_target_properties(ImageContainer PROPERTIES PUBLIC_HEADER "${ImageContainerHeaders}")
install(TARGETS ImageContainer EXPORT ImageContainerTargets
            RUNTIME DESTINATION bin/FIPP
            LIBRARY DESTINATION lib/FIPP
            PUBLIC_HEADER DESTINATION include/FIPP/ImageContainer)
install(
                EXPORT ImageContainerTargets 
                FILE ImageContainerTargets.cmake
                DESTINATION "lib/cmake/FIPP/ImageContainer"
            )
include(CMakePackageConfigHelpers)

configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/ImageContainerConfig.cmake.in
            "${CMAKE_CURRENT_BINARY_DIR}/ImageContainerConfig.cmake"
            INSTALL_DESTINATION "lib/cmake/FIPP/ImageContainer"
            NO_SET_AND_CHECK_MACRO
            NO_CHECK_REQUIRED_COMPONENTS_MACRO
            )
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/ImageContainerConfigVersion.cmake"
  VERSION "${ImageContainer_VERSION_MAJOR}.${ImageContainer_VERSION_MINOR}"
  COMPATIBILITY AnyNewerVersion
)

install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/ImageContainerConfig.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/ImageContainerConfigVersion.cmake
  DESTINATION lib/cmake/FIPP/ImageContainer
  )

  export(EXPORT ImageContainerTargets
  FILE "${CMAKE_CURRENT_BINARY_DIR}/ImageContainerTargets.cmake"
)