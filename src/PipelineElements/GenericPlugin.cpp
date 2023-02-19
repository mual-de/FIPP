#include "PipelineElements/GenericPlugin.hpp"
#include <chrono>
#define LOG(lvl, msg) this->m_log->log(lvl, msg, "GenericPlugin", __func__, __LINE__, this->m_frameNumber);

using namespace FIPP::pipe;
using namespace FIPP::logging;
GenericPlugin::GenericPlugin(std::string elemName, int elemId, std::shared_ptr<FIPP::logging::ILogger> log) : m_elemName(elemName), m_elemId(elemId)
{
    m_log = log;
    m_frameNumber = 0;
    m_newImgArrived = false;
    m_stop = false;
    m_filterActivated = INPUT_FILTER_ALLOW_ALL_INPUTS;
    m_state = ElementState::IDLE;
}

GenericPlugin::~GenericPlugin()
{
    // Stop running thread before dtor
    if (this->m_state == ElementState::RUNNING)
    {
        LOG(LogLevel::CONFIG, "Stop Thread");
        this->stopThread();
    }
}

void GenericPlugin::addImageToInputPipe(std::shared_ptr<img::ImageContainer> img)
{
    std::lock_guard lk(this->m_inputLockMutex);
    this->m_inputQueue.push(img);
    this->m_newImgArrived = true;
    this->m_cvNotify.notify_one();
}

void GenericPlugin::startThread()
{
    LOG(LogLevel::CONFIG, "Start Thread");
    this->m_state = ElementState::STARTING;
    this->m_stop = false;
    this->m_newImgArrived = false;
    this->m_workerThread = std::thread(&GenericPlugin::run, this);
}

bool GenericPlugin::stopThread()
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

void GenericPlugin::run()
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
            std::shared_ptr<img::ImageContainer> retImg = this->doCalculation(actImg);
            LOG(LogLevel::CONFIG, "Send to sucessors");
            this->sendImageToSucessors(retImg);
            
        }
    }
    LOG(LogLevel::CONFIG, "set in not running state!");
}