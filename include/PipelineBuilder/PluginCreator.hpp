/**
 * @file PluginCreator.hpp
 * @author Alexander Mueller (dev@alexandermaxmueller.de)
 * @brief Creates plugins based on dynamic loaded shared objects.
 * @version 0.1
 * @date 2023-02-25
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef __PLUGIN_CREATOR__
#define __PLUGIN_CREATOR__
#include <exception>
#include <memory>
#include <string>
#include "../PipelineElements/IGenericPipelineElement.hpp"
namespace YAML
{
    class Node;
};

namespace FIPP
{

    /**
     * @brief function pointer to make pipeline pointer which is necessary for every plugin
     * 
     */
    typedef void (*makePipeElement_t)(std::shared_ptr<FIPP::pipe::IGenericPipelineElement> & pipeElemPtr, YAML::Node config, std::string elementName, int elemId, std::shared_ptr<FIPP::logging::ILogger> log);


    class PluginCreator
    {
    public:
        /**
         * @brief Construct a new Plugin Creator object
         * 
         * @param pcConfig Configuration node for plugin creator
         * @param log shared logger to store output from pc
         */
        PluginCreator(YAML::Node pcConfig, std::shared_ptr<FIPP::logging::ILogger> log);
        ~PluginCreator();
        /**
         * @brief Get the Pipeline Element object
         * 
         * @param peId pipeline element identifier
         * @param config configuration for this plugin
         * @param elementName name of this element for easier identification
         * @param elemId unique id to identify in pipelines
         * @param log logger to store output
         * @throws invalid_argument if peId is unknown
         * @return std::shared_ptr<FIPP::pipe::IGenericPipelineElement> 
         */
        std::shared_ptr<FIPP::pipe::IGenericPipelineElement> getPipelineElement(std::string peId, YAML::Node config, std::string elementName, int elemId, std::shared_ptr<FIPP::logging::ILogger> log);
        /**
         * @brief Get the List Of Installed Plugins identifiers
         * 
         * @return std::vector<std::string> 
         */
        std::vector<std::string> getListOfInstalledPlugins();
    private:
        /**
         * @brief map of pipeline element identifier and the corresponding make pipeline element function call
         * 
         */
        std::map<std::string, makePipeElement_t> m_plugins;
        /**
         * @brief Vector of handles created from dlopen/dlsym, stored to call dlclose while destroying pc.
         * 
         */
        std::vector<void*> m_soHandles;
        /**
         * @brief logger to store output data
         * 
         */
        std::shared_ptr<FIPP::logging::ILogger> m_log;
        /**
         * @brief List of dirs to look for plugins
         * 
         */
        std::vector<std::string> m_dirs;
        /**
         * @brief Open a shared object library
         * @throw runtime_error if not able to load plugin
         * @param file 
         */
        void loadSharedObject(const std::string file);
        /**
         * @brief Load all libraries from a given directory
         * @throw invalid_argument if path is not a valid
         * @param path 
         */
        void loadLibsFromDir(const std::string path);


    };

}

#endif