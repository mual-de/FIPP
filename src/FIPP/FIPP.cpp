/**
 * @file FIPP.cpp
 * @author Alexander Mueller (dev@alexandermaxmueller.de)
 * @brief A small application to load a yaml config file and run a pipeline
 * @version 0.1
 * @date 2023-03-04
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <string>
#include <memory>
#include <iostream>
#include <filesystem>
#include <exception>
#include "PipelineBuilder/PipelineBuilder.hpp"
#include <yaml-cpp/yaml.h>

using namespace std;
namespace fs = filesystem;

int main(int argc, char *argv[])
{
    string path = string(argv[1]);
    cout << "Incomming path: " << path << endl;
    if (!fs::is_regular_file(path))
    {
        cout << "File: " << path << " doesn't exist - stop now" << endl;
        return 0;
    }

    YAML::Node config = YAML::LoadFile(path);
    cout << "Loaded config as yaml" << endl;
    unique_ptr<FIPP::PipelineBuilder> pb;
    try
    {
        pb = make_unique<FIPP::PipelineBuilder>(config);
        cout << "Created pb" << endl;
    }
    catch (std::exception &e)
    {
        cout << "Can't create pipeline due to exception: " << e.what() << endl;
        return 0;
    }
    if (pb->interrogatePipeline())
    {
        cout << "Succcessfull interrogated pipeline!" << endl;
        pb->startPipelines();
        cout << "Pipelines are running now!" << endl;
        cout << "Stop pipelines by pressing any key; " << endl;
        int a = cin.get();
        cout << "Try to stop pipelines" << endl;
        pb->stopPipelines();
        cout << "Pipelines stopped!" << endl;
    }
    else
    {
        cout << "error" << endl;
    }

    return 0;
}