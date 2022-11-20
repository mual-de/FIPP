project(PipelineElements)
add_library(PipelineElements GenericSource.cpp GenericSink.cpp GenericPlugin.cpp GenericPluginSiSo.cpp GenericPluginSiSoWP.cpp InputFilter.cpp)
target_include_directories(PipelineElements PUBLIC "${PROJECT_SOURCE_DIR}/../../include")
find_package(yaml-cpp)
target_link_libraries(PipelineElements PRIVATE yaml-cpp Point ImageContainer Logging)
set(PipelineElementsHeaders
    "${PROJECT_SOURCE_DIR}/../../include/PipelineElements/GenericPlugin.hpp"
    "${PROJECT_SOURCE_DIR}/../../include/PipelineElements/GenericPluginSiSo.hpp"
    "${PROJECT_SOURCE_DIR}/../../include/PipelineElements/GenericPluginSiSoWP.hpp"
    "${PROJECT_SOURCE_DIR}/../../include/PipelineElements/GenericSink.hpp"
    "${PROJECT_SOURCE_DIR}/../../include/PipelineElements/GenericSource.hpp"
    "${PROJECT_SOURCE_DIR}/../../include/PipelineElements/IGenericPipelineElement.hpp"
    "${PROJECT_SOURCE_DIR}/../../include/PipelineElements/IGenericPlugin.hpp"
    "${PROJECT_SOURCE_DIR}/../../include/PipelineElements/IGenericSink.hpp"
    "${PROJECT_SOURCE_DIR}/../../include/PipelineElements/IGenericSource.hpp"
    "${PROJECT_SOURCE_DIR}/../../include/PipelineElements/InputFilter.hpp"
)
set_target_properties(PipelineElements PROPERTIES PUBLIC_HEADER "${PipelineElementsHeaders}")
install(TARGETS PipelineElements 
            RUNTIME DESTINATION bin/FIPP
            LIBRARY DESTINATION lib/FIPP
            PUBLIC_HEADER DESTINATION include/FIPP/PipelineElements)