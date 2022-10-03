add_library(Logging FileLogger.cpp TestLogger.cpp UnitTestLogger.cpp)
target_include_directories(Logging PUBLIC "${PROJECT_SOURCE_DIR}/include")