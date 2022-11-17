find_package(yaml-cpp)
add_library(pipelineElements SHARED GenericSource.cpp GenericSink.cpp GenericPlugin.cpp GenericPluginSiSo.cpp GenericPluginSiSoWP.cpp InputFilter.cpp)
target_include_directories(pipelineElements PUBLIC "${PROJECT_SOURCE_DIR}/include")
target_link_libraries(pipelineElements PRIVATE yaml-cpp Point ImageContainer Logging)