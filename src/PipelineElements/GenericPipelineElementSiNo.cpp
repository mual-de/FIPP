#include "PipelineElements/GenericPipelineElementSiNo.hpp"

using namespace FIPP::pipe;

GenericPipelineElementSiNo::GenericPipelineElementSiNo(std::string elemName, int elemId, std::shared_ptr<FIPP::logging::ILogger> log) : m_elemName(elemName), m_elemId(elemId)
{
    m_log = log;
    m_frameNumber = 0;
    m_newImgArrived = false;
    m_stop = false;
    m_isRunning = false;
}

void GenericPipelineElementSiNo::addImageToInputPipe(std::shared_ptr<img::ImageContainer> img)
{
    std::lock_guard lk(this->m_inputLockMutex);
    this->m_inputQueue.push(img);
    this->m_newImgArrived = true;
    this->m_cvNotify.notify_one();
}

void GenericPipelineElementSiNo::startThread()
{
    this->m_workerThread = std::thread(&GenericPipelineElementSiNo::run, this);
    this->m_isRunning = true;
}

void GenericPipelineElementSiNo::run()
{
    while (!this->m_stop)
    {
        std::unique_lock lk(this->m_inputLockMutex);
        this->m_cvNotify.wait(lk, []
                              { return true; });
        if(!this->m_stop){ break;}
        if(this->m_newImgArrived){
            std::shared_ptr<img::ImageContainer> actImg = this->m_inputQueue.front();
            this->m_inputQueue.pop();
            lk.unlock();
            this->m_frameNumber = actImg->getFrameNumber();
            this->doCalculation(actImg);
        } 
    }
    this->m_isRunning = false;
}