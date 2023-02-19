#include "PipelineElements/GenericPluginSiSoWP.hpp"
#include "PipelineElements/IGenericSink.hpp"
#include "ImageContainer/ImageFormat.hpp"
#include "Logging/ILogging.hpp"
using namespace FIPP::pipe;
using namespace FIPP::logging;

#define LOG(lvl, msg) this->m_log->log(lvl, msg, "GenericPluginSiSo", __func__, __LINE__, this->m_frameNumber);

GenericPluginSiSoWP::GenericPluginSiSoWP(std::string elemName, int elemId, std::shared_ptr<FIPP::logging::ILogger> log) : GenericPlugin(elemName, elemId, log)
{
    LOG(LogLevel::ERROR, "Started PipelineElement");
    this->m_predecessorId = -1;
}

StartState GenericPluginSiSoWP::startElement(int predecessorId)
{
    LOG(LogLevel::ERROR, "predecessor id: " + std::to_string(predecessorId));
    LOG(LogLevel::ERROR, "predecessor internal id: " + std::to_string(m_predecessorId));
    if (predecessorId != this->m_predecessorId)
    {
        LOG(LogLevel::ERROR, "Wrong predecessor");
        return StartState::WRONG_PREDECESSOR;
    }
    if(this->m_pool.get() != nullptr){
        LOG(LogLevel::INFO, "pool hasn't been reseted yet, reset is done by starter");
        this->m_pool.reset();
    }
    this->m_pool = std::make_unique<img::ImagePool>(m_poolSize, m_outputConfiguration);
    this->startThread();
    // Needed for tests without connected output!
    if (this->m_successor != nullptr)
    {
        return this->m_successor->startElement(this->m_elemId);
    }else{
        return StartState::STARTED;
    }
    return StartState::START_ERROR;
}


StopState GenericPluginSiSoWP::stopElement()
{
    this->stopThread();
    LOG(LogLevel::ERROR, "stop running element");
    if (this->m_successor != nullptr)
    {
        return this->m_successor->stopElement();
    }
    this->m_pool.reset();
    if(this->m_successor == nullptr){
        return StopState::STOPPED;
    }
    return StopState::STOP_ERROR;
}

void GenericPluginSiSoWP::connectPredecessor(int elemId)
{
    this->m_predecessorId = elemId;
}

void GenericPluginSiSoWP::connectSuccessor(std::shared_ptr<IGenericSink> elem)
{
    this->m_successor = elem;
}

void GenericPluginSiSoWP::sendImageToSucessors(std::shared_ptr<img::ImageContainer> img)
{
    // Needed for testruns
    if (this->m_successor != nullptr)
    {
        this->m_successor->addImageToInputPipe(img);
    }
}


bool GenericPluginSiSoWP::interogateConnection(img::ImageContainerConfig imgConfig, int predecessorId)
    {
    if (this->m_filterActivated)
    {
        if (this->m_filter.checkIfCompatible(imgConfig))
        {
            if (this->m_successor != nullptr)
            {
                return this->m_successor->interogateConnection(this->m_outputConfiguration, this->m_elemId);
            }
            LOG(LogLevel::ERROR, "Image Formats are not compatibel!");
            return true;
        }
        return false;
    }
    if (this->m_successor != nullptr)
    {
        return this->m_successor->interogateConnection(this->m_outputConfiguration, this->m_elemId);
    }
    return true;
}