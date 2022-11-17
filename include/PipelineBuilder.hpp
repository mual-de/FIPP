/**
 * @file PluginBuilder.hpp
 * @author Alexander Mueller (dev@alexandermaxmueller.de)
 * @brief Creates PipelineElements by a given configuration
 * @version 0.1
 * @date 2022-09-24
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef _PLUGINBUILDER_HPP__
#define _PLUGINBUILDER_HPP__
#include "PipelineElements/IGenericPipelineElement.hpp"
#include "Logging/ILogging.hpp"
#include <memory>
#include <set>
#include <string>
namespace FIPP
{
    namespace pipe
    {
        /**
         * @brief Definitions of errors during creation of pipeline elements.
         *
         */
        typedef enum e_pipelineError
        {
            OKAY,
            UNKNOWN_PIPENAME,
            ID_ALREADY_IN_USE,
            MISSING_PARAMETERS
        } PipelineError;
        
        class PipelineBuilder
        {
        public:
            PipelineBuilder(std::shared_ptr<logging::ILogger> log, std::vector<std::string> path2plugins);
            /**
             * @brief Get the Pipeline Element object specified by given parameters
             *
             * @param id unique id of this element
             * @param elementName
             * @param pipeElem
             * @return PipelineError
             */
            virtual PipelineError getPipelineElement(const int id, const std::string elementName, std::shared_ptr<IGenericPipelineElement> &pipeElem);

        private:
            /**
             * @brief Standard system logger from central controller
             *
             */
            std::shared_ptr<logging::ILogger> m_log;
            /**
             * @brief set of uuids already in use by pipeline.
             *
             */
            std::set<int> m_idsInUsage;
            /**
             * @brief map of available plugins and their correspondent constructors.
            */
            std::map<std::string, void*> m_plugins;
        };
    }
}
#endif