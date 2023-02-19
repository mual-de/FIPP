#ifndef __GENERIC_SINK_SI_HPP__
#define __GENERIC_SINK_SI_HPP__
#include "GenericSink.hpp"

#include "../Logging/ILogging.hpp"
#define LOG(lvl, msg) this->m_log->log(lvl, msg, "GenericSinkSi", __func__, __LINE__, this->m_frameNumber);
namespace FIPP
{
    namespace pipe
    {
        class GenericSinkSi : public GenericSink
        {
        public:
            inline GenericSinkSi(std::string elemName, int elemId, std::shared_ptr<FIPP::logging::ILogger> log) : GenericSink(elemName, elemId, log){};
            inline ~GenericSinkSi()
            {
                // Stop running thread before dtor
                if (this->m_state == ElementState::RUNNING)
                {
                    LOG(logging::LogLevel::CONFIG, "Stop Thread");
                    this->stopThread();
                };
            }
            StartState startElement(int predecessorId)
            {
                if (predecessorId == this->m_predeccessor)
                {
                    this->initializeInterfaces();
                    this->startThread();
                    return StartState::STARTED;
                }
                else
                {
                    return StartState::WRONG_PREDECESSOR;
                }
            };
            StopState stopElement()
            {
                this->stopThread();
                return StopState::STOPPED;
            };

            void connectPredecessor(int elemId)
            {
                if (this->m_predeccessor == -1)
                {
                    this->m_predeccessor = elemId;
                }
                else
                {
                    if (this->m_predeccessor == elemId)
                    {
                        LOG(FIPP::logging::WARNING, "connect predecessor called twice!");
                    }
                    else
                    {
                        LOG(FIPP::logging::ERROR, "Element id already set");
                        throw std::invalid_argument("Element id already set!");
                    }
                }
            };
            bool interogateConnection(img::ImageContainerConfig imgConfig, int predecessorId);
        private:
            int m_predeccessor = -1;
            
        };
    };
};

#endif