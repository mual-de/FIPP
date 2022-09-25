#ifndef __GENERIC_PIPELINE_ELEMENT_HPP__
#define __GENERIC_PIPELINE_ELEMENT_HPP__
#include "IGenericPipelineElement.hpp"
#include "../Logging/ILogging.hpp"
#include <memory>
#include <string>
#include <mutex>
#include <queue>

namespace FIPP
{
    namespace pipe
    {
        class GenericPipelineElement : public IGenericPipelineElement
        {
        public:
            GenericPipelineElement(std::string elemName, int elemId, std::shared_ptr<FIPP::logging::ILogger> log);
            virtual bool startElement(img::ImageContainerConfig imgConfig, int predecessorId);
            virtual bool stopElement();
            void addImageToInputPipe(std::shared_ptr<img::ImageContainer> img);
                        /**
             * @brief get the object name set by the derived class (e.g. crop-plugin).
             * 
             * @return std::string 
             */
            inline std::string getName() const {return this->m_elemName;};
            /**
             * @brief Get the unique id of this pipeline element.
             * 
             * @return int 
             */
            inline int getId() const {return this->m_elemId;};

        protected:
            const std::string m_elemName;
            const int m_elemId;
            std::shared_ptr<FIPP::logging::ILogger> m_log;
            unsigned long long int m_frameNumber;

            std::mutex m_inputLockMutex;
            std::queue<std::shared_ptr<img::ImageContainer>> m_inputQueue;

            bool m_isRunning;

            virtual void sendImageToSucessors(std::shared_ptr<img::ImageContainer> img) = 0;
            /**
             * @brief Overwrite by derived plugin to do calculation on next image
             * 
             * @param img 
             */
            virtual void doCalculation(std::shared_ptr<img::ImageContainer> img) = 0;
        };

    };
};

#endif