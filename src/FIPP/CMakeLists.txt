project(FIPP-APPLICATION)

find_package(yaml-cpp)

add_executable(fipp_run FIPP.cpp)

target_link_libraries(fipp_run PUBLIC yaml-cpp PipelineBuilder)
install(TARGETS fipp_run 
            EXPORT PipelineBuilderTargets
            RUNTIME DESTINATION bin)
            