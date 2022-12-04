project(Point)
add_library(Point SHARED Point.cpp)
find_package(yaml-cpp)
target_include_directories(Point PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../include>  
$<INSTALL_INTERFACE:include/FIPP/Point>)
target_link_libraries(Point PRIVATE yaml-cpp)
set_target_properties(Point PROPERTIES PUBLIC_HEADER "${PROJECT_SOURCE_DIR}/../../include/Point.hpp")
install(TARGETS Point EXPORT PointTargets
            RUNTIME DESTINATION bin/FIPP
            LIBRARY DESTINATION lib/FIPP
            PUBLIC_HEADER DESTINATION include/FIPP)

            install(
                EXPORT PointTargets 
                FILE PointTargets.cmake
                DESTINATION "lib/cmake/FIPP/Point"
            )
include(CMakePackageConfigHelpers)

configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/PointConfig.cmake.in
            "${CMAKE_CURRENT_BINARY_DIR}/PointConfig.cmake"
            INSTALL_DESTINATION "lib/cmake/FIPP/Point"
            NO_SET_AND_CHECK_MACRO
            NO_CHECK_REQUIRED_COMPONENTS_MACRO
            )
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/PointConfigVersion.cmake"
  VERSION "${Point_VERSION_MAJOR}.${Point_VERSION_MINOR}"
  COMPATIBILITY AnyNewerVersion
)

install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/PointConfig.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/PointConfigVersion.cmake
  DESTINATION lib/cmake/FIPP/Point
  )

  export(EXPORT PointTargets
  FILE "${CMAKE_CURRENT_BINARY_DIR}/PointTargets.cmake"
)