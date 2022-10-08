#include "PipelineElements/GenericPipelineElementSiSo.hpp"
#include "ImageContainer/ImageFormat.hpp"
#include "Logging/ILogging.hpp"
using namespace FIPP::pipe;
using namespace FIPP::logging;

#define LOG(lvl, msg) this->m_log->log(lvl, msg, "GenericPipelineElementSISO", __func__, __LINE__, this->m_frameNumber);

GenericPipelineElementSiSo::GenericPipelineElementSiSo(std::string elemName, int elemId, std::shared_ptr<FIPP::logging::ILogger> log) : GenericPipelineElement(elemName, elemId, log)
{
    LOG(LogLevel::ERROR, "Started PipelineElement");
    this->m_predecessorId = -1;
}

bool GenericPipelineElementSiSo::startElement(FIPP::img::ImageContainerConfig imgConfig, int predecessorId)
{
    LOG(LogLevel::ERROR, "predecessor id: " + std::to_string(predecessorId));
    LOG(LogLevel::ERROR, "predecessor internal id: " + std::to_string(m_predecessorId));
    if (predecessorId != this->m_predecessorId)
    {
        LOG(LogLevel::ERROR, "Wrong predecessor");
        return false;
    }
    this->startThread();
    // Needed for tests without connected output!
    if (this->m_successor != nullptr)
    {
        return this->m_successor->startElement(imgConfig, this->m_elemId);
    }
    return true;
}

bool GenericPipelineElementSiSo::stopElement()
{
    this->stopThread();
    LOG(LogLevel::ERROR, "stop running element");
    if(this->m_successor != nullptr){
        return this->m_successor->stopElement();
    }
    return true;
}

void GenericPipelineElementSiSo::connectPredecessor(int elemId)
{
    this->m_predecessorId = elemId;
}

void GenericPipelineElementSiSo::connectSuccessor(std::shared_ptr<IGenericPipelineElement> elem)
{
    this->m_successor = elem;
}

void GenericPipelineElementSiSo::sendImageToSucessors(std::shared_ptr<img::ImageContainer> img)
{
    // Needed for testruns
    if (this->m_successor != nullptr)
    {
        this->m_successor->addImageToInputPipe(img);
    }
}
