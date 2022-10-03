#ifndef __ILOGGING_HPP__
#define __ILOGGING_HPP__
#include <string>
namespace FIPP
{
    namespace logging
    {
        typedef enum e_logLevel
        {
            ERROR,
            WARNING,
            DEBUG,
            CONFIG,
            INFO
        } LogLevel;


        
        class ILogger
        {
        public:
            virtual void log(LogLevel lvl, std::string msg, std::string fName, std::string fnName, int lineNum, unsigned long long int frameNumber) = 0;
            virtual void setLevel(LogLevel lvl) = 0;
            virtual void enable() = 0;
            virtual void disable() = 0;
            virtual bool getStatus() = 0;
        };
    }
}

#endif