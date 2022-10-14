#ifndef __GENERIC__SOURCE_HPP__
#define __GENERIC__SOURCE_HPP__

#include <memory>
#include <string>
#include <thread>
#include <atomic>
#include "../Logging/ILogging.hpp"
#include "IGenericSource.hpp"
#include "../ImageContainer/ImageFormat.hpp"
#include "../ImageContainer/ImageContainer.hpp"
namespace FIPP
{

    namespace pipe
    {
        class GenericSource : public IGenericSource
        {
        public:
            GenericSource(std::string elemName, int elemId, std::shared_ptr<FIPP::logging::ILogger> log);
            ~GenericSource();
            virtual bool startElement(img::ImageContainerConfig imgConfig, int predecessorId) = 0;
            virtual bool stopElement() = 0;
            /**
             * @brief get the object name set by the derived class (e.g. crop-plugin).
             *
             * @return std::string
             */
            inline std::string getName() const { return this->m_elemName; };
            /**
             * @brief Get the unique id of this pipeline element.
             *
             * @return int
             */
            inline int getId() const { return this->m_elemId; };

            inline ElementTypes getElementType() { return ElementTypes::SOURCE; };
            inline ElementState getState() {return this->m_state;};

        protected:
            const std::string m_elemName;
            const int m_elemId;
            /**
             * @brief store every log information
             *
             */
            std::shared_ptr<FIPP::logging::ILogger> m_log;
            /**
             * @brief Actual frame number that is processed by this pipeline Element
             * perfect for debugging reasons!
             *
             */
            unsigned long long int m_frameNumber;

            std::mutex m_inputLockMutex;
            /**
             * @brief Variable to stop running worker loop.
             *
             */
            std::atomic_bool m_stop;
            ElementState m_state;
            /**
             * @brief Thread to run the doCalculation function.
             *
             */
            std::thread m_workerThread;
            /**
             * @brief Overwrite by derived plugin to do calculation on next image
             *
             * @param img
             */
            virtual void doCalculation(std::shared_ptr<img::ImageContainer> img) = 0;
            void startThread();
            bool stopThread();

        private:
            void run();
        };
    };

};

#endif