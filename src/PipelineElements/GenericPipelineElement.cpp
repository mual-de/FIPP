#include "PipelineElements/GenericPipelineElement.hpp"

using namespace FIPP::pipe;

GenericPipelineElement::GenericPipelineElement(std::string elemName, int elemId, std::shared_ptr<FIPP::logging::ILogger> log): m_elemName(elemName), m_elemId(elemId){
    m_log = log;
    m_frameNumber = 0;
}


void GenericPipelineElement::addImageToInputPipe(std::shared_ptr<img::ImageContainer> img){
    this->m_inputLockMutex.lock();
    this->m_inputQueue.push(img);
    this->m_inputLockMutex.unlock();
    // Todo notify thread for processing
}

