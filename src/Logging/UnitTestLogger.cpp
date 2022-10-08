#include "Logging/UnitTestLogger.hpp"
#include <iostream>

using namespace FIPP::logging;

UnitTestLogger::UnitTestLogger(LogLevel lvl)
{
    m_minlvl = lvl;
    m_isEnabled = true;
}

void UnitTestLogger::log(LogLevel lvl, std::string msg, std::string fName, std::string fnName, int lineNum, unsigned long long int frameNumber)
{
    if (m_isEnabled)
    {
        if (lvl <= m_minlvl)
        {
            UnitTestEntry entry {lvl, msg, fName, fnName, lineNum, frameNumber};
            this->m_mutex.lock();
            this->m_entries.push_back(entry);
            this->m_mutex.unlock();
        }
    }
}