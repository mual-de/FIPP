#include "PipelineElements/GenericSink.hpp"
#include "ImageContainer/ImageFormat.hpp"
#include "Logging/ILogging.hpp"
using namespace FIPP::pipe;
using namespace FIPP::logging;

#define LOG(lvl, msg) this->m_log->log(lvl, msg, "GenericSink", __func__, __LINE__, this->m_frameNumber);

GenericSink::GenericSink(std::string elemName, int elemId, std::shared_ptr<FIPP::logging::ILogger> log) : m_elemName(elemName), m_elemId(elemId)
{
    m_log = log;
    m_frameNumber = 0;
    m_newImgArrived = false;
    m_stop = false;
    m_filterActivated = false;
    m_state = ElementState::IDLE;
}

GenericSink::~GenericSink()
{
    // Stop running thread before dtor
    if (this->m_state == ElementState::RUNNING)
    {
        LOG(LogLevel::CONFIG, "Stop Thread");
        this->stopThread();
    }
}


bool GenericSink::stopThread()
{
    this->m_stop = true;
    this->m_state = ElementState::STOPPING;
    LOG(LogLevel::CONFIG, "Notify thread");
    this->m_cvNotify.notify_all();
    LOG(LogLevel::CONFIG, "Wait for join");
    this->m_workerThread.join();
    this->m_state = ElementState::IDLE;
    return true;
};

void GenericSink::startThread()
{
    LOG(LogLevel::CONFIG, "Start Thread");
    this->m_state = ElementState::STARTING;
    this->m_stop = false;
    this->m_newImgArrived = false;
    this->m_workerThread = std::thread(&GenericSink::run, this);
}

void GenericSink::addImageToInputPipe(std::shared_ptr<img::ImageContainer> img)
{
    std::lock_guard lk(this->m_inputLockMutex);
    LOG(LogLevel::DEBUG, "Added image to queue");
    this->m_inputQueue.push(img);
    this->m_newImgArrived = true;
    this->m_cvNotify.notify_one();
}




void GenericSink::run()
{
    LOG(LogLevel::CONFIG, "Started Thread");
    this->m_state = ElementState::RUNNING;
    while (!this->m_stop)
    {
        std::unique_lock lk(this->m_inputLockMutex);
        LOG(LogLevel::CONFIG, "Wait for notify");

        this->m_cvNotify.wait(lk, [this]()
                              { return (m_newImgArrived || m_stop); });
        LOG(LogLevel::CONFIG, "Notified");
        if (this->m_stop)
        {
            LOG(LogLevel::CONFIG, "Check if stopped");
            break;
        }
        if (this->m_newImgArrived || !this->m_inputQueue.empty())
        {
            LOG(LogLevel::CONFIG, "New msg available");
            std::shared_ptr<img::ImageContainer> actImg = this->m_inputQueue.front();
            this->m_inputQueue.pop();
            this->m_newImgArrived = false;
            lk.unlock();
            this->m_frameNumber = actImg->getFrameNumber();
            this->doCalculation(actImg);
            // Free Image because sink is the last element in pipeline
            actImg->setUnBound();
            LOG(LogLevel::CONFIG, "Send to sucessors");
            
        }
    }
    LOG(LogLevel::CONFIG, "set in not running state!");
}