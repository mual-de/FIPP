#ifndef __GENERIC_PIPELINE_ELEMENT_SISO_HPP__
#define __GENERIC_PIPELINE_ELEMENT_SISO_HPP__
#include "GenericPipelineElement.hpp"
#include "../Logging/ILogging.hpp"
#include <memory>
#include <string>
#include <mutex>
#include <queue>

namespace FIPP
{
    namespace pipe
    {
        class GenericPipelineElementSiSo : public GenericPipelineElement
        {
        public:
            GenericPipelineElementSiSo(std::string elemName, int elemId, std::shared_ptr<FIPP::logging::ILogger> log);
            virtual ~GenericPipelineElementSiSo(){};
            bool startElement(img::ImageContainerConfig imgConfig, int predecessorId);
            bool stopElement();
            void connectPredecessor(int elemId);
            void connectSuccessor(std::shared_ptr<IGenericPipelineElement> elem);

        protected:
            std::shared_ptr<IGenericPipelineElement> m_successor;
            int m_predecessorId;

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