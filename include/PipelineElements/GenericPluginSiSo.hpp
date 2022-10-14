#ifndef __GENERIC_PLUGIN_SISO_HPP__
#define __GENERIC_PLUGIN_SISO_HPP__
#include "GenericPlugin.hpp"
#include "../Logging/ILogging.hpp"
#include <memory>
#include <string>
#include <mutex>
#include <queue>
#include "InputFilter.hpp"

namespace FIPP
{
    namespace pipe
    {
        class GenericPluginSiSo : public GenericPlugin
        {
        public:
            GenericPluginSiSo(std::string elemName, int elemId, std::shared_ptr<FIPP::logging::ILogger> log);
            virtual ~GenericPluginSiSo(){};
            bool startElement(int predecessorId);
            bool stopElement();
            void connectPredecessor(int elemId);
            void connectSuccessor(std::shared_ptr<IGenericSink> elem);
            bool interogateConnection(img::ImageContainerConfig imgConfig, int predecessorId);

        protected:
            std::shared_ptr<IGenericSink> m_successor;
            int m_predecessorId;
            InputFilter m_filter;
            void sendImageToSucessors(std::shared_ptr<img::ImageContainer> img);
            /**
             * @brief Overwrite by derived plugin to do calculation on next image
             *
             * @param img
             */
            virtual std::shared_ptr<img::ImageContainer> doCalculation(std::shared_ptr<img::ImageContainer> img) = 0;

        };

    };
};

#endif