#include "PipelineBuilder/PluginCreator.hpp"
#include <iostream>
#include <filesystem>
#include <dlfcn.h>
#include <FIPP/PipelineElements/IGenericPipelineElement.hpp>
#include <FIPP/Logging/ILogging.hpp>
#include <yaml-cpp/yaml.h>

using namespace FIPP;
using namespace std;
namespace fs = filesystem;

#define LOG(lvl, msg) this->m_log->log(lvl, msg, "PluginCreator", __func__, __LINE__, 0);

/**
 * @brief Get pipeline element/plugin identifier via dlsym.
 *
 */
typedef void (*getElementIdentifier_t)(std::string &name);

PluginCreator::PluginCreator(YAML::Node pcConfig, shared_ptr<FIPP::logging::ILogger> log)
{
    this->m_log = log;
    // Adding the standard path
    this->m_dirs.push_back("/usr/local/lib/FIPP-Plugins");

    if (pcConfig["additionalPluginDir"])
    {
        YAML::Node addPlugDirs = pcConfig["additionalPluginDir"];
        if (addPlugDirs.IsSequence())
        {
            for (size_t i = 0; i < addPlugDirs.size(); i++)
            {
                this->m_dirs.push_back(addPlugDirs[i].as<string>());
            }
        }
        else
        {
            this->m_dirs.push_back(addPlugDirs.as<string>());
        }
    }

    for (const auto &dir : this->m_dirs)
    {
        try
        {
            this->loadLibsFromDir(dir);
        }
        catch (invalid_argument e)
        {
            LOG(logging::LogLevel::ERROR, e.what());
        }
    }
}

shared_ptr<FIPP::pipe::IGenericPipelineElement> PluginCreator::getPipelineElement(std::string peId, YAML::Node config, std::string elementName, int elemId, std::shared_ptr<FIPP::logging::ILogger> log)
{
    if (m_plugins.find(peId) == m_plugins.end())
    {
        throw invalid_argument("Plugin not in plugins list!");
    }
    shared_ptr<FIPP::pipe::IGenericPipelineElement> pE;

    m_plugins[peId](pE, config, elementName, elemId, log);
    LOG(logging::LogLevel::CONFIG, "Created new plugin of type: " + peId + " with id: " + to_string(pE->getId()) + " name: " + pE->getName());
    return pE;
}

PluginCreator::~PluginCreator()
{
    for (const auto &handle : this->m_soHandles)
    {
        dlclose(handle);
    }
}

vector<string> PluginCreator::getListOfInstalledPlugins()
{
    vector<string> retVal;
    for (map<string, makePipeElement_t>::iterator it = m_plugins.begin(); it != m_plugins.end(); it++)
    {
        retVal.push_back(it->first);
    }
    return retVal;
}

void PluginCreator::loadSharedObject(const string file)
{
    void *handle = dlopen(file.c_str(), RTLD_NOW);
    if (!handle)
    {
        throw runtime_error("Can't open library: " + string(dlerror()));
    }

    getElementIdentifier_t getElementIdentifier = (getElementIdentifier_t)dlsym(handle, "getElementIdentifier");
    const char *dlsymErr = dlerror();
    if (dlsymErr)
    {
        dlclose(handle);
        throw runtime_error("Can't call getElementIdentifier: " + string(dlsymErr));
    }

    string id;
    getElementIdentifier(id);

    makePipeElement_t makePipelineElement = (makePipeElement_t)dlsym(handle, "makePipeElement");

    dlsymErr = dlerror();
    if (dlsymErr)
    {
        dlclose(handle);
        throw runtime_error("Can't call makePipeElement: " + string(dlsymErr));
    }

    this->m_soHandles.push_back(handle);
    this->m_plugins[id] = makePipelineElement;
}

void PluginCreator::loadLibsFromDir(const string path)
{
    if (!fs::exists(path))
    {
        throw invalid_argument("Path does not exist!");
    }
    if (!fs::is_directory(path))
    {
        throw invalid_argument("Path is not a directory!");
    }
    for (const auto &entry : fs::directory_iterator(path))
    {
        // Todo: Check for compability!
        if (entry.path().extension() == ".so")
        {
            try
            {
                this->loadSharedObject(entry.path());
            }
            catch (runtime_error e)
            {
                LOG(logging::LogLevel::ERROR, e.what());
            }
        }
    }
}