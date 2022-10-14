add_library(pipelineElements GenericSource.cpp GenericPlugin.cpp GenericPluginSiSo.cpp InputFilter.cpp)
target_include_directories(pipelineElements PUBLIC "${PROJECT_SOURCE_DIR}/include")
target_link_libraries(pipelineElements PRIVATE Point ImageContainer Logging)