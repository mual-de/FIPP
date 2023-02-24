#ifndef __PLUGIN_CREATOR__
#define __PLUGIN_CREATOR__
#include <exception>

namespace FIPP
{
    namespace errors
    {
        struct PluginNotAvailable : public std::exception
        {
            const char *what() const throw()
            {
                return "No free image available in pool";
            }
        };
        struct PluginNotCompatible : public std::exception
        {
            const char *what() const throw()
            {
                return "No free image available in pool";
            }
        };
    };
    typedef enum e_PCDirState{
        ADDED,
        EMPTY_DIR
    } PCDirState;
    class PluginCreator
    {
    public:
        PluginCreator();
        PCDirState addDirectory(std::string path);
        std::map<std::string, std::string
    };

}

#endif