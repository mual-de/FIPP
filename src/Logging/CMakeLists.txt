project(Logging)
add_library(Logging SHARED FileLogger.cpp TestLogger.cpp UnitTestLogger.cpp)
target_include_directories(Logging PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../../include>  
$<INSTALL_INTERFACE:include/FIPP/Logging>)
set(LoggingHeaders
    "${PROJECT_SOURCE_DIR}/../../include/Logging/FileLogger.hpp"
    "${PROJECT_SOURCE_DIR}/../../include/Logging/ILogging.hpp"
    "${PROJECT_SOURCE_DIR}/../../include/Logging/TestLogger.hpp"
    "${PROJECT_SOURCE_DIR}/../../include/Logging/UnitTestLogger.hpp"
)
set_target_properties(Logging PROPERTIES PUBLIC_HEADER "${LoggingHeaders}")
install(TARGETS Logging 
            EXPORT LoggingTargets
            RUNTIME DESTINATION bin/FIPP
            LIBRARY DESTINATION lib/FIPP
            PUBLIC_HEADER DESTINATION include/FIPP/Logging)

            install(
                EXPORT LoggingTargets 
                FILE LoggingTargets.cmake
                DESTINATION "lib/cmake/FIPP/Logging"
            )
include(CMakePackageConfigHelpers)

configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/LoggingConfig.cmake.in
            "${CMAKE_CURRENT_BINARY_DIR}/LoggingConfig.cmake"
            INSTALL_DESTINATION "lib/cmake/FIPP/Logging"
            NO_SET_AND_CHECK_MACRO
            NO_CHECK_REQUIRED_COMPONENTS_MACRO
            )
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/LoggingConfigVersion.cmake"
  VERSION "${Logging_VERSION_MAJOR}.${Logging_VERSION_MINOR}"
  COMPATIBILITY AnyNewerVersion
)

install(FILES
  ${CMAKE_CURRENT_BINARY_DIR}/LoggingConfig.cmake
  ${CMAKE_CURRENT_BINARY_DIR}/LoggingConfigVersion.cmake
  DESTINATION lib/cmake/FIPP/Logging
  )

  export(EXPORT LoggingTargets
  FILE "${CMAKE_CURRENT_BINARY_DIR}/LoggingTargets.cmake"
)