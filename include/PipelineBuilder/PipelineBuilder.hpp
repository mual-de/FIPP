/**
 * @file PipelineBuilder.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2023-03-01
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef __PIPELINE_CONTROLLER_HPP__
#define __PIPELINE_CONTROLLER_HPP__

#include <memory>
#include <vector>
#include <map>
#include <set>
#include <string>
#include "PipelineElements/IGenericPipelineElement.hpp"

namespace YAML
{
    class Node;
};

namespace FIPP{
    class PluginCreator;

    namespace pipe{
        class IGenericSource;
        class IGenericSink;
        class IGenericPlugin;
        class ExternalInput;
    }
    namespace img
    {
        class ImagePool;
        class ImageContainer;
    }
    namespace logging
    {
        class ILogger;
    } // namespace name
    
    class PipelineBuilder{
        public:
            PipelineBuilder(YAML::Node config);
            ~PipelineBuilder();
            /**
             * @brief Get external input object to induce images from other applications to a pipe
             * 
             * @param id to identify external input element in pipe
             * @throws std::invalid_argument exception if id is not a external input
             * @return std::shared_ptr<FIPP::pipe::ExternalInput> 
             */
            std::shared_ptr<FIPP::pipe::ExternalInput> getExternalInput(int id); 
            /**
             * @brief Start all pipeline elements in correct order
             * @throws std::runtime_error if this is not possible
             * 
             */
            void startPipelines();
            /**
             * @brief Stop all pipeline elements in correct order
             * @throws std::runtime_error is this is not possible
             * 
             */
            void stopPipelines();

            /**
             * @brief Check if all pipeline elements are compatible and can connect with each other.
             * @warning Needs to run before startPipeline to set input/output configuration depending on settings.
             * @return true its possible to connect all elements
             * @return false its impossible to connect all elements -> pipe will not start!
             */
            bool interrogatePipeline();
        private:
            /**
             * @brief Connect all pipeline Elements with each other to create an end to end pipe
             * @throw std::runtime_error if its not possible to connect all elements!
             */
            void connectElements(const YAML::Node config);
            /**
             * @brief Connect a sink with inputs
             * 
             * @param sinkConfig yaml configuration for sink
             * @param sink shared_ptr to object
             * @throw std::invalid_argument if yaml config is not valid
             */
            void connectSink(const YAML::Node sinkConfig, std::shared_ptr<pipe::IGenericSink> sink);
            /**
             * @brief Connect inputs/outputs of a plugin
             * 
             * @param plgConfig
             * @param plugin shared_ptr to plugin for configuration
             * @throw std::invalid_argument if yaml config is not valid
             */
            void connectPlugin(const YAML::Node plgConfig, std::shared_ptr<pipe::IGenericPlugin> plugin);
            /**
             * @brief Create all pipelineElements via PluginCreator
             * @see PluginCreator
             * 
             * @param config 
             */
            void createElements(YAML::Node config);
            std::map<int, std::string> m_uuid2name;
            std::map<int, std::shared_ptr<FIPP::pipe::IGenericPipelineElement>> m_pipeElements;
            std::vector<std::shared_ptr<FIPP::pipe::IGenericSource>> m_srcElements;
            std::vector<std::shared_ptr<FIPP::pipe::IGenericSink>> m_sinkElements;
            /**
             * @brief Path to store logging informations
             * 
             * By default "/tmp"
             * 
             */
            std::string m_logPath;
            /**
             * @brief A set containing all ids leading to an external input
             * 
             */
            std::set<int> m_externalInputs;
            /**
             * @brief Indicates if the pipeline is already running.
             * 
             */
            bool m_currentlyRunning;

            std::unique_ptr<PluginCreator> m_pc;
            std::shared_ptr<logging::ILogger> m_log;
            std::string m_ts;
    };
}

#endif