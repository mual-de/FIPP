#include "PipelineElements/GenericSource.hpp"
#include "PipelineElements/IGenericSink.hpp"
#include "ImageContainer/ImageFormat.hpp"
#include "Logging/ILogging.hpp"
using namespace FIPP::pipe;
using namespace FIPP::logging;

#define LOG(lvl, msg) this->m_log->log(lvl, msg, "GenericPluginSiSo", __func__, __LINE__, this->m_frameNumber);

GenericSource::GenericSource(std::string elemName, int elemId, std::shared_ptr<FIPP::logging::ILogger> log) : m_elemName(elemName), m_elemId(elemId)
{
    m_log = log;
    m_frameNumber = 0;
    m_stop = false;
    m_state = ElementState::IDLE;
}

GenericSource::~GenericSource()
{
    // Stop running thread before dtor
    if (this->m_state == ElementState::RUNNING)
    {
        LOG(LogLevel::CONFIG, "Stop Thread");
        this->stopThread();
    }
}