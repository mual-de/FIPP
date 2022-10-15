add_library(ImageContainer ImageContainer.cpp ImageContainerCPU.cpp ImagePool.cpp)
target_include_directories(ImageContainer PUBLIC "${PROJECT_SOURCE_DIR}/include")