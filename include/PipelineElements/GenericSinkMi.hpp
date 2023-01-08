#ifndef __GENERIC_SINK_MI_HPP__
#define __GENERIC_SINK_MI_HPP__
#include "GenericSink.hpp"
#include "../Logging/ILogging.hpp"
#define LOG(lvl, msg) this->m_log->log(lvl, msg, "GenericSinkMi", __func__, __LINE__, this->m_frameNumber);
namespace FIPP
{
    namespace pipe
    {
        class GenericSinkMi : public GenericSink
        {
        public:
            GenericSinkMi(std::string elemName, int elemId, std::shared_ptr<FIPP::logging::ILogger> log) : GenericSink(elemName, elemId, log){};
            ~GenericSinkMi()
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
                int internalId = -1;
                for (int i = 0; i < this->m_predecessorIds.size(); i++)
                {
                    if (this->m_predecessorIds.at(i) == predecessorId)
                    {
                        internalId = i;
                        break;
                    }
                }
                if (internalId == -1)
                {
                    return StartState::WRONG_PREDECESSOR;
                }
                else
                {
                    this->m_predecessorsStartFlag[internalId] = true;
                }
                bool ret_val = true;
                for (const auto startFlag : this->m_predecessorsStartFlag)
                {
                    ret_val &= startFlag;
                }
                if (ret_val)
                {
                    return StartState::STARTED;
                }
                return StartState::PENDING;
            };
            StopState stopElement()
            {
                this->stopThread();
                return StopState::STOPPED;
            };

        private:
            std::vector<int> m_predecessorIds;
            std::vector<bool> m_predecessorsStartFlag;
        };
    };
};

#endif