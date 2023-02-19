#include "PipelineElements/GenericPluginSiSo.hpp"
#include "PipelineElements/IGenericSink.hpp"
#include "ImageContainer/ImageFormat.hpp"
#include "Logging/ILogging.hpp"
using namespace FIPP::pipe;
using namespace FIPP::logging;

#define LOG(lvl, msg) this->m_log->log(lvl, msg, "GenericPluginSiSo", __func__, __LINE__, this->m_frameNumber);

GenericPluginSiSo::GenericPluginSiSo(std::string elemName, int elemId, std::shared_ptr<FIPP::logging::ILogger> log) : GenericPlugin(elemName, elemId, log)
{
    LOG(LogLevel::ERROR, "Started PipelineElement");
    this->m_predecessorId = -1;
}

StartState GenericPluginSiSo::startElement(int predecessorId)
{
    LOG(LogLevel::ERROR, "predecessor id: " + std::to_string(predecessorId));
    LOG(LogLevel::ERROR, "predecessor internal id: " + std::to_string(m_predecessorId));
    if (predecessorId != this->m_predecessorId)
    {
        LOG(LogLevel::ERROR, "Wrong predecessor");
        return StartState::WRONG_PREDECESSOR;
    }
    this->startThread();
    // Needed for tests without connected output!
    if (this->m_successor != nullptr)
    {
        return this->m_successor->startElement(this->m_elemId);
    }
    return StartState::START_ERROR;
}

bool GenericPluginSiSo::interogateConnection(img::ImageContainerConfig imgConfig, int predecessorId)
    {
    if (this->m_filterActivated)
    {
        if (this->m_filter.checkIfCompatible(imgConfig))
        {
            if (this->m_successor != nullptr)
            {
                return this->m_successor->interogateConnection(imgConfig, this->m_elemId);
            }
            LOG(LogLevel::ERROR, "Image Formats are not compatibel!");
            return true;
        }
        return false;
    }
    if (this->m_successor != nullptr)
    {
        return this->m_successor->interogateConnection(imgConfig, this->m_elemId);
    }
    return true;
}

StopState GenericPluginSiSo::stopElement()
{
    this->stopThread();
    LOG(LogLevel::INFO, "stop running element");
    if (this->m_successor != nullptr)
    {
        return this->m_successor->stopElement();
    }
    return StopState::STOPPED;
}

void GenericPluginSiSo::connectPredecessor(int elemId)
{
    this->m_predecessorId = elemId;
}

void GenericPluginSiSo::connectSuccessor(std::shared_ptr<IGenericSink> elem)
{
    this->m_successor = elem;
}

void GenericPluginSiSo::sendImageToSucessors(std::shared_ptr<img::ImageContainer> img)
{
    // Needed for testruns
    if (this->m_successor != nullptr)
    {
        this->m_successor->addImageToInputPipe(img);
    }
}
