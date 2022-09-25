#include "PipelineElements/GenericPipelineElementSiSo.hpp"
#include "ImageContainer/ImageFormat.hpp"
#include "Logging/ILogging.hpp"
using namespace FIPP::pipe;
using namespace FIPP::logging;

#define LOG(lvl, msg) this->m_log->log(lvl, msg, __FILE__, __func__, __LINE__, this->m_frameNumber);

GenericPipelineElementSiSo::GenericPipelineElementSiSo(std::string elemName, int elemId, std::shared_ptr<FIPP::logging::ILogger> log) : GenericPipelineElement(elemName, elemId, log)
{
    LOG(LogLevel::ERROR, "Wrong predecessor");
}

bool GenericPipelineElementSiSo::startElement(FIPP::img::ImageContainerConfig imgConfig, int predecessorId)
{
    if(predecessorId != this->m_predecessorId){
        LOG(LogLevel::ERROR, "Wrong predecessor");
        return false;
    }
    return this->m_successor->startElement(imgConfig, this->m_elemId);
}

bool GenericPipelineElementSiSo::stopElement() {
    this->m_isRunning = false;
    LOG(LogLevel::ERROR, "stop running element");
    return this->m_successor->stopElement();
}

void GenericPipelineElementSiSo::connectPredecessor(int elemId){
    this->m_predecessorId = elemId;
}

void GenericPipelineElementSiSo::connectSuccessor(std::shared_ptr<IGenericPipelineElement> elem){
    this->m_successor = elem;
}