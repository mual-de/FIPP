#ifndef __FILE_LOGGER_HPP__
#define __FILE_LOGGER_HPP__
#include "ILogging.hpp"
#include <string>
#include <mutex>
#include <iostream>
#include <fstream>

namespace FIPP
{
    namespace logging
    {
        class FileLogger : public ILogger
        {
        public:
            FileLogger(std::string pathToFile, LogLevel minLvl);
            ~FileLogger();
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
            std::ofstream m_ofstream;
        };
    }
}

#endif