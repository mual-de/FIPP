/**
 * @file UnitTestLogger.hpp
 * @author Alexander Mueller (dev@alexandermaxmueller.de)
 * @brief Logger for automatic unit testing (log outputs can be read via getEntries)
 * @version 0.1
 * @date 2022-10-03
 * 
 * Use only for unit testing with 10-50 log outputs. This is not intended for productive use!
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef __UNIT_TEST_LOGGER_HPP__
#define __UNIT_TEST_LOGGER_HPP__
#include "ILogging.hpp"
#include <string>
#include <mutex>
#include <vector>
namespace FIPP
{
    namespace logging
    {
        typedef struct e_UnitTestEntry
        {
            LogLevel level;
            std::string msg;
            std::string fName;
            std::string fnName;
            int lineNum;
            unsigned long long int frameNumber;
        } UnitTestEntry;

        class UnitTestLogger : public ILogger
        {
        public:
            UnitTestLogger(LogLevel minLvl);
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
            inline std::vector<UnitTestEntry> getEntries();

        private:
            bool m_isEnabled;
            LogLevel m_minlvl;
            std::mutex m_mutex;
            std::vector<UnitTestEntry> m_entries;
        };
    }
}

#endif