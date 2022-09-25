#ifndef __TEST_LOGGER_HPP__
#define __TEST_LOGGER_HPP__
#include "ILogging.hpp"
#include <string>
#include <mutex>
namespace FIPP
{
    namespace logging
    {
        class TestLogger : public ILogger
        {
        public:
            TestLogger(LogLevel minLvl);
            void log(LogLevel lvl, std::string msg, std::string fName, std::string fnName, int lineNum, unsigned long long int frameNumber);
            inline void setLevel(LogLevel lvl)
            {
                m_mutex.lock();
                m_minlvl = lvl;
                m_mutex.unlock();
            };
            inline void enable()
            {
                m_mutex.lock();
                m_isEnabled = true;
                m_mutex.unlock();
            };
            inline void disable()
            {
                m_mutex.lock();
                m_isEnabled = false;
                m_mutex.unlock();
            };
            inline bool getStatus() { return m_isEnabled; };

        private:
            bool m_isEnabled;
            LogLevel m_minlvl;
            std::mutex m_mutex;
        };
    }
}

#endif