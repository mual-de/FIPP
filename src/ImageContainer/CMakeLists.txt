add_library(ImageContainer ImageContainer.cpp ImageContainerCPU.cpp)
target_include_directories(ImageContainer PUBLIC "${PROJECT_SOURCE_DIR}/src/include")