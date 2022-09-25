#include "Logging/TestLogger.hpp"
#include <iostream>

using namespace FIPP::logging;

TestLogger::TestLogger(LogLevel lvl)
{
    m_minlvl = lvl;
}

void TestLogger::log(LogLevel lvl, std::string msg, std::string fName, std::string fnName, int lineNum, unsigned long long int frameNumber)
{
    if (m_isEnabled)
    {
        if (lvl <= m_minlvl)
        {
            std::cout << "Frame: " << frameNumber << " - Level: " << lvl << " - File: " << fName << " - Function: " << fnName << " - Message: " << msg << std::endl;
        }
    }
}