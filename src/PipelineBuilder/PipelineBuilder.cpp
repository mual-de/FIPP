#include "PipelineBuilder/PipelineBuilder.hpp"
#include "PipelineElements/IGenericSource.hpp"
#include "PipelineElements/IGenericSink.hpp"
#include "PipelineElements/IGenericPlugin.hpp"
#include "PipelineElements/ExternalInput.hpp"
#include "PipelineBuilder/PluginCreator.hpp"
#include "Logging/FileLogger.hpp"
#include <yaml-cpp/yaml.h>
#include <ctime>
using namespace FIPP;
using namespace FIPP::pipe;
#define LOG(lvl, msg) this->m_log->log(lvl, msg, __FILE__, __func__, __LINE__, 0);

PipelineBuilder::PipelineBuilder(YAML::Node config)
{
    m_currentlyRunning = false;
    if (config["logPath"])
    {
        m_logPath = config["logPath"].as<std::string>();
    }
    else
    {
        m_logPath = "/tmp";
    }
    this->m_ts = std::to_string(std::time(0));
    this->m_log = std::make_shared<logging::FileLogger>(m_logPath + "/" + this->m_ts + "-pipelineBuilder.log", logging::LogLevel::INFO);
    this->m_pc = std::make_unique<PluginCreator>(config, this->m_log);

    this->createElements(config["pipelines"]);
    this->connectElements(config["pipelines"]);
}
PipelineBuilder::~PipelineBuilder()
{
    if (m_currentlyRunning)
    {
        this->stopPipelines();
    }
}

void PipelineBuilder::connectSink(const YAML::Node node, std::shared_ptr<pipe::IGenericSink> sink)
{

    int id = node["id"].as<int>();
    if (sink->getPluginType() == PluginTypes::SISO)
    {
        if (node["input"].IsDefined())
        {
            if (node["input"].IsScalar())
            {
                int inputId = node["input"].as<int>();
                LOG(logging::LogLevel::CONFIG, "Set input of sink with id: " + std::to_string(id) + " to elementId: " + std::to_string(inputId));
                sink->connectPredecessor(inputId);
                std::shared_ptr<IGenericPipelineElement> pE = this->m_pipeElements.at(inputId);
                if (pE->getElementType() == ElementTypes::PLUGIN)
                {
                    std::shared_ptr<IGenericPlugin> plg = std::static_pointer_cast<FIPP::pipe::IGenericPlugin>(pE);
                    LOG(logging::LogLevel::CONFIG, "Set sink with id: " + std::to_string(id) + " as output of plugin with id: " + std::to_string(inputId));
                    plg->connectSuccessor(sink);
                }
                else if (pE->getElementType() == ElementTypes::SOURCE)
                {
                    std::shared_ptr<IGenericSource> src = std::static_pointer_cast<FIPP::pipe::IGenericSource>(pE);
                    LOG(logging::LogLevel::CONFIG, "Set sink with id: " + std::to_string(id) + " as output of source with id: " + std::to_string(inputId));
                    src->connectSuccessor(sink);
                }
            }
            else
            {
                throw std::invalid_argument("input is not scalar for siso sink: " + std::to_string(id));
            }
        }
        else
        {
            throw std::invalid_argument("input is not defined for sink: " + std::to_string(id));
        }
    }
    else
    {
        if (node["input"].IsDefined())
        {
            throw std::invalid_argument("input is not defined for sink: " + std::to_string(id));
        }
        if (node["input"].IsScalar())
        {
            int inputId = node["input"].as<int>();
            sink->connectPredecessor(inputId);
            std::shared_ptr<IGenericPipelineElement> pE = this->m_pipeElements.at(inputId);
            LOG(logging::LogLevel::CONFIG, "Set input of sink with id: " + std::to_string(id) + " to elementId: " + std::to_string(inputId));
            if (pE->getElementType() == ElementTypes::PLUGIN)
            {
                std::shared_ptr<IGenericPlugin> plg = std::static_pointer_cast<FIPP::pipe::IGenericPlugin>(pE);
                LOG(logging::LogLevel::CONFIG, "Set sink with id: " + std::to_string(id) + " as output of plugin with id: " + std::to_string(inputId));
                plg->connectSuccessor(sink);
            }
            else if (pE->getElementType() == ElementTypes::SOURCE)
            {
                std::shared_ptr<IGenericSource> src = std::static_pointer_cast<FIPP::pipe::IGenericSource>(pE);
                LOG(logging::LogLevel::CONFIG, "Set sink with id: " + std::to_string(id) + " as output of source with id: " + std::to_string(inputId));
                src->connectSuccessor(sink);
            }
        }
        // In case of multiple inputs!
        else if (node["input"].IsSequence())
        {
            for (std::size_t i = 0; i < node["input"].size(); i++)
            {
                int inputId = node["input"][i].as<int>();
                sink->connectPredecessor(inputId);
                LOG(logging::LogLevel::CONFIG, "Set input of sink with id: " + std::to_string(id) + " to elementId: " + std::to_string(inputId));
                std::shared_ptr<IGenericPipelineElement> pE = this->m_pipeElements.at(inputId);
                if (pE->getElementType() == ElementTypes::PLUGIN)
                {
                    std::shared_ptr<IGenericPlugin> plg = std::static_pointer_cast<FIPP::pipe::IGenericPlugin>(pE);
                    LOG(logging::LogLevel::CONFIG, "Set sink with id: " + std::to_string(id) + " as output of plugin with id: " + std::to_string(inputId));
                    plg->connectSuccessor(sink);
                }
                else if (pE->getElementType() == ElementTypes::SOURCE)
                {
                    std::shared_ptr<IGenericSource> src = std::static_pointer_cast<FIPP::pipe::IGenericSource>(pE);
                    LOG(logging::LogLevel::CONFIG, "Set sink with id: " + std::to_string(id) + " as output of source with id: " + std::to_string(inputId));
                    src->connectSuccessor(sink);
                }
            }
        }
        else
        {
            throw std::invalid_argument("input is not well defined for miso sink: " + std::to_string(id));
        }
    }
}

void PipelineBuilder::connectPlugin(const YAML::Node node, std::shared_ptr<pipe::IGenericPlugin> plugin)
{
    int id = node["id"].as<int>();
    PluginTypes type = plugin->getPluginType();
    // Connect single input
    if (type == PluginTypes::SISO || type == PluginTypes::SIMO)
    {
        if (node["input"].IsDefined())
        {
            if (node["input"].IsScalar())
            {
                int inputId = node["input"].as<int>();
                plugin->connectPredecessor(inputId);
                LOG(logging::LogLevel::CONFIG, "Set input of plugin with id: " + std::to_string(id) + " to elementId: " + std::to_string(inputId));
                std::shared_ptr<IGenericPipelineElement> pE = this->m_pipeElements.at(inputId);
                if (pE->getElementType() == ElementTypes::PLUGIN)
                {
                    std::shared_ptr<IGenericPlugin> plg = std::static_pointer_cast<FIPP::pipe::IGenericPlugin>(pE);
                    LOG(logging::LogLevel::CONFIG, "Set plugin with id: " + std::to_string(id) + " as output of plugin with id: " + std::to_string(inputId));
                    plg->connectSuccessor(std::static_pointer_cast<FIPP::pipe::IGenericSink>(plugin));
                }
                else if (pE->getElementType() == ElementTypes::SOURCE)
                {
                    std::shared_ptr<IGenericSource> src = std::static_pointer_cast<FIPP::pipe::IGenericSource>(pE);
                    LOG(logging::LogLevel::CONFIG, "Set plugin with id: " + std::to_string(id) + " as output of source with id: " + std::to_string(inputId));
                    src->connectSuccessor(std::static_pointer_cast<FIPP::pipe::IGenericSink>(plugin));
                }
            }
            else
            {
                throw std::invalid_argument("input is not scalar for siso/simo plugin: " + std::to_string(id));
            }
        }
        else
        {
            throw std::invalid_argument("input is not defined for plugin: " + std::to_string(id));
        }
    }
    // Connect multiple input
    else
    {
        if (node["input"].IsDefined())
        {
            throw std::invalid_argument("input is not defined for plugin: " + std::to_string(id));
        }
        if (node["input"].IsScalar())
        {
            int inputId = node["input"].as<int>();
            plugin->connectPredecessor(inputId);
            LOG(logging::LogLevel::CONFIG, "Set input of plugin with id: " + std::to_string(id) + " to elementId: " + std::to_string(inputId));
            std::shared_ptr<IGenericPipelineElement> pE = this->m_pipeElements.at(inputId);
            if (pE->getElementType() == ElementTypes::PLUGIN)
            {
                std::shared_ptr<IGenericPlugin> plg = std::static_pointer_cast<FIPP::pipe::IGenericPlugin>(pE);
                LOG(logging::LogLevel::CONFIG, "Set plugin with id: " + std::to_string(id) + " as output of plugin with id: " + std::to_string(inputId));
                plg->connectSuccessor(std::static_pointer_cast<FIPP::pipe::IGenericSink>(plugin));
            }
            else if (pE->getElementType() == ElementTypes::SOURCE)
            {
                std::shared_ptr<IGenericSource> src = std::static_pointer_cast<FIPP::pipe::IGenericSource>(pE);
                LOG(logging::LogLevel::CONFIG, "Set plugin with id: " + std::to_string(id) + " as output of source with id: " + std::to_string(inputId));
                src->connectSuccessor(std::static_pointer_cast<FIPP::pipe::IGenericSink>(plugin));
            }
        }
        // In case of multiple inputs!
        else if (node["input"].IsSequence())
        {
            for (std::size_t i = 0; i < node["input"].size(); i++)
            {
                int inputId = node["input"][i].as<int>();
                plugin->connectPredecessor(inputId);
                LOG(logging::LogLevel::CONFIG, "Set input of plugin with id: " + std::to_string(id) + " to elementId: " + std::to_string(inputId));
                std::shared_ptr<IGenericPipelineElement> pE = this->m_pipeElements.at(inputId);
                if (pE->getElementType() == ElementTypes::PLUGIN)
                {
                    std::shared_ptr<IGenericPlugin> plg = std::static_pointer_cast<FIPP::pipe::IGenericPlugin>(pE);
                    LOG(logging::LogLevel::CONFIG, "Set plugin with id: " + std::to_string(id) + " as output of plugin with id: " + std::to_string(inputId));
                    plg->connectSuccessor(std::static_pointer_cast<FIPP::pipe::IGenericSink>(plugin));
                }
                else if (pE->getElementType() == ElementTypes::SOURCE)
                {
                    std::shared_ptr<IGenericSource> src = std::static_pointer_cast<FIPP::pipe::IGenericSource>(pE);
                    LOG(logging::LogLevel::CONFIG, "Set plugin with id: " + std::to_string(id) + " as output of source with id: " + std::to_string(inputId));
                    src->connectSuccessor(std::static_pointer_cast<FIPP::pipe::IGenericSink>(plugin));
                }
            }
        }
        else
        {
            throw std::invalid_argument("input is not well defined for miso/mimo plugin: " + std::to_string(id));
        }
    }
}

void PipelineBuilder::connectElements(const YAML::Node config)
{
    for (std::size_t i = 0; i < config.size(); i++)
    {
        YAML::Node element = config[i];

        if (element["element"])
        {
            YAML::Node node = element["element"];
            int id = node["id"].as<int>();
            std::shared_ptr<pipe::IGenericPipelineElement> elem = this->m_pipeElements.at(id);
            // only sinks and plugins have inputs defined
            if (elem->getElementType() == pipe::ElementTypes::SINK)
            {
                std::shared_ptr<pipe::IGenericSink> sink = std::static_pointer_cast<FIPP::pipe::IGenericSink>(elem);
                try
                {
                    this->connectSink(node, sink);
                }
                catch (std::invalid_argument &e)
                {
                    throw std::runtime_error("Can't connect elements - error: " + std::string(e.what()));
                }
            }
            else if (elem->getElementType() == pipe::ElementTypes::PLUGIN)
            {
                std::shared_ptr<pipe::IGenericPlugin> plugin = std::static_pointer_cast<FIPP::pipe::IGenericPlugin>(elem);
                try
                {
                    this->connectPlugin(node, plugin);
                }
                catch (std::invalid_argument &e)
                {
                    throw std::runtime_error("Can't connect elements - error: " + std::string(e.what()));
                }
            }
        }
    }
}

void PipelineBuilder::createElements(YAML::Node config)
{
    if (config.IsSequence())
    {
        for (std::size_t i = 0; i < config.size(); i++)
        {
            YAML::Node element = config[i];
            if (element["external"])
            {
                YAML::Node node = element["external"];
                int id = node["id"].as<int>();
                std::shared_ptr<logging::ILogger> log = std::make_shared<logging::FileLogger>(m_logPath + "/" + this->m_ts + "-External-" + std::to_string(id) + ".log", logging::LogLevel::INFO);

                std::shared_ptr<pipe::IGenericPipelineElement> elem = std::make_shared<ExternalInput>(node, id, log);

                this->m_srcElements.push_back(std::static_pointer_cast<FIPP::pipe::IGenericSource>(elem));
                this->m_pipeElements[id] = elem;
            }
            else if (element["element"])
            {
                YAML::Node node = element["element"];
                int id = node["id"].as<int>();
                std::string peId = node["type"].as<std::string>();
                std::string name = node["name"].as<std::string>();

                std::shared_ptr<logging::ILogger> log = std::make_shared<logging::FileLogger>(m_logPath + "/" + this->m_ts + "-" + name + "-" + std::to_string(id) + ".log", logging::LogLevel::INFO);
                std::shared_ptr<pipe::IGenericPipelineElement> elem = this->m_pc->getPipelineElement(peId, node, name, id, log);

                if (elem->getElementType() == ElementTypes::SOURCE)
                {
                    this->m_srcElements.push_back(std::static_pointer_cast<FIPP::pipe::IGenericSource>(elem));
                }
                else if (elem->getElementType() == ElementTypes::SINK)
                {
                    this->m_sinkElements.push_back(std::static_pointer_cast<FIPP::pipe::IGenericSink>(elem));
                }
                this->m_pipeElements[id] = elem;
            }
            else
            {
                throw std::invalid_argument("Unknown parameter in pipelines list!");
            }
        }
    }
    else
    {
        throw std::invalid_argument("config need to be a sequence!");
    }
}

std::shared_ptr<FIPP::pipe::ExternalInput> PipelineBuilder::getExternalInput(int id)
{
    if (m_externalInputs.find(id) != m_externalInputs.end())
    {
        return std::static_pointer_cast<FIPP::pipe::ExternalInput>(this->m_pipeElements[id]);
    }
    throw std::invalid_argument("No external input with id: " + std::to_string(id) + " is available!");
}

void PipelineBuilder::startPipelines()
{
    if (m_currentlyRunning)
    {
        throw std::runtime_error("Already started!");
    }
    m_currentlyRunning = true;
    bool completeState = true;
    for (const auto &e : this->m_srcElements)
    {
        StartState state = e->startElement(-1);
        if (state == StartState::STARTED)
        {
            completeState = completeState && true;
        }else{
            completeState = completeState && false;
        }
    }
    if (!completeState)
    {
        m_currentlyRunning = false;
        throw std::runtime_error("Can't start pipeline, something went wrong!");
    }
}

void PipelineBuilder::stopPipelines()
{
    if (!m_currentlyRunning)
    {
        throw std::runtime_error("Already stopped pipeline");
    }
    for (const auto &e : this->m_srcElements)
    {
        e->stopElement();
    }
    m_currentlyRunning = false;
}

bool PipelineBuilder::interrogatePipeline()
{
    bool ret = true;
    for (const auto &e : this->m_srcElements)
    {
        ret = ret && e->interogateConnection();
    }
    return ret;
}