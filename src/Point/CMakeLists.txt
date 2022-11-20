project(Point)
add_library(Point SHARED Point.cpp)
find_package(yaml-cpp)
target_include_directories(Point PUBLIC "${PROJECT_SOURCE_DIR}/../../include")
target_link_libraries(Point PRIVATE yaml-cpp)
set_target_properties(Point PROPERTIES PUBLIC_HEADER "${PROJECT_SOURCE_DIR}/../../include/Point.hpp")
install(TARGETS Point 
            RUNTIME DESTINATION bin/FIPP
            LIBRARY DESTINATION lib/FIPP
            PUBLIC_HEADER DESTINATION include/FIPP/Point)