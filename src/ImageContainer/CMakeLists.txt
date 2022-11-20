project(ImageContainer)
add_library(ImageContainer SHARED ImageContainer.cpp ImageContainerCPU.cpp ImagePool.cpp MetaDataSystem.cpp ImageFormat.cpp)
find_package(yaml-cpp)
target_include_directories(ImageContainer PUBLIC "${PROJECT_SOURCE_DIR}/../../include")
target_link_libraries(ImageContainer PRIVATE yaml-cpp)
set(ImageContainerHeaders
    "${PROJECT_SOURCE_DIR}/../../include/ImageContainer/ImageContainer.hpp"
    "${PROJECT_SOURCE_DIR}/../../include/ImageContainer/ImageContainerCPU.hpp"
    "${PROJECT_SOURCE_DIR}/../../include/ImageContainer/ImageFormat.hpp"
    "${PROJECT_SOURCE_DIR}/../../include/ImageContainer/ImagePool.hpp"
    "${PROJECT_SOURCE_DIR}/../../include/ImageContainer/MetaDataSystem.hpp"
)
set_target_properties(ImageContainer PROPERTIES PUBLIC_HEADER "${ImageContainerHeaders}")
install(TARGETS ImageContainer 
            RUNTIME DESTINATION bin/FIPP
            LIBRARY DESTINATION lib/FIPP
            PUBLIC_HEADER DESTINATION include/FIPP/ImageContainer)