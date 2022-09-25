/**
 * @file FileLogger.cpp
 * @author Alexander Mueller (dev@alexandermaxmueller.de)
 * @brief Log directly in txt file
 * @version 0.1
 * @date 2022-09-24
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#include "Logging/FileLogger.hpp"

using namespace FIPP::logging;

FileLogger::FileLogger(std::string pathToFile, LogLevel minLvl)
{
    this->m_minlvl = minLvl;
    this->m_isEnabled = true;
    this->m_ofstream.open(pathToFile);
    this->m_ofstream << "Frame, LogLevel, Source File, Function, Line, Msg" << std::endl;
}
void FileLogger::log(LogLevel lvl, std::string msg, std::string fName, std::string fnName, int lineNum, unsigned long long int frameNumber)
{
    if (m_isEnabled)
    {
        if (lvl <= m_minlvl)
        {
            this->m_mutex.lock();

            this->m_ofstream << frameNumber << "," << lvl << "," << fName << "," << fnName << "," << lineNum << "," << msg << std::endl;
            this->m_mutex.unlock();
        }
    }
}

FileLogger::~FileLogger()
{
    this->m_isEnabled = false;
    this->m_ofstream.close();
}