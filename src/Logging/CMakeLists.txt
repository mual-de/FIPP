project(Logging)
add_library(Logging SHARED FileLogger.cpp TestLogger.cpp UnitTestLogger.cpp)
target_include_directories(Logging PUBLIC "${PROJECT_SOURCE_DIR}/../../include")
set(LoggingHeaders
    "${PROJECT_SOURCE_DIR}/../../include/Logging/FileLogger.hpp"
    "${PROJECT_SOURCE_DIR}/../../include/Logging/ILogging.hpp"
    "${PROJECT_SOURCE_DIR}/../../include/Logging/TestLogger.hpp"
    "${PROJECT_SOURCE_DIR}/../../include/Logging/UnitTestLogger.hpp"
)
set_target_properties(Logging PROPERTIES PUBLIC_HEADER "${LoggingHeaders}")
install(TARGETS Logging 
            RUNTIME DESTINATION bin/FIPP
            LIBRARY DESTINATION lib/FIPP
            PUBLIC_HEADER DESTINATION include/FIPP/Logging)