#include "PipelineElements/GenericSource.hpp"
#include "ImageContainer/ImageFormat.hpp"
#include "Logging/ILogging.hpp"
#include <math.h>
using namespace FIPP::pipe;
using namespace FIPP::logging;

#define LOG(lvl, msg) this->m_log->log(lvl, msg, "GenericSource", __func__, __LINE__, this->m_frameNumber);

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

void GenericSource::startThread()
{
    LOG(LogLevel::CONFIG, "Start Thread");
    this->m_fps_duration = std::chrono::milliseconds(static_cast<int>(ceil((1/this->m_fps)*1000)));
    this->initializeInterfaces();
    this->m_state = ElementState::STARTING;
    this->m_stop = false;
    this->m_workerThread = std::thread(&GenericSource::run, this);
}

bool GenericSource::stopThread()
{
    this->m_stop = true;
    this->m_state = ElementState::STOPPING;
    LOG(LogLevel::CONFIG, "Wait for join");
    this->m_workerThread.join();
    this->closeInterfaces();
    this->m_state = ElementState::IDLE;
    return true;
};

void GenericSource::run()
{
    LOG(LogLevel::CONFIG, "Started Thread");
    this->m_state = ElementState::RUNNING;
    while (!this->m_stop)
    {
        auto sleepTimer = std::chrono::steady_clock::now() + this->m_fps_duration;
        // get next free image
        std::shared_ptr<img::ImageContainer> img = this->m_pool->getNextFreeImage();
        // implemented by derived class
        this->doCalculation(img);
        // and out to pipe
        this->m_successor->addImageToInputPipe(img);
        std::this_thread::sleep_until(sleepTimer);
    }
    LOG(LogLevel::CONFIG, "set in not running state!");
}

StartState GenericSource::startElement(int predecessorId)
{
    LOG(LogLevel::INFO, "start element");
    this->startThread();
    // Needed for tests without connected output!
    if (this->m_successor != nullptr)
    {
        return this->m_successor->startElement(this->m_elemId);
    }
    return StartState::STARTED;
}
StopState GenericSource::stopElement()
{
    this->stopThread();
    LOG(LogLevel::ERROR, "stop running element");
    if (this->m_successor != nullptr)
    {
        return this->m_successor->stopElement();
    }
    return StopState::STOPPED;
};