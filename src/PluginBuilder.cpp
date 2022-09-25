#include "PluginBuilder.hpp"

using namespace FIPP::pipe;
#define LOG(lvl, msg) this->m_log->log(lvl, msg, __FILE__, __func__, __LINE__, 0);

PipelineBuilder::PipelineBuilder(std::shared_ptr<logging::ILogger> log)
{
    this->m_log = log;
}

PipelineError PipelineBuilder::getPipelineElement(const int id, const std::string elementName, std::shared_ptr<IGenericPipelineElement> &pipeElem)
{
    if (this->m_idsInUsage.count(id))
    {
        LOG(FIPP::logging::LogLevel::CONFIG, "PipelineElement ID: " + std::to_string(id) + " already in use");
        return PipelineError::ID_ALREADY_IN_USE;
    }
    else
    {
        // In progress...
        LOG(FIPP::logging::LogLevel::CONFIG, "Unknown pipeline element name: " + elementName);
        return PipelineError::UNKNOWN_PIPENAME;
    }
}