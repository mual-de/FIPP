#ifndef __GENERIC_PIPELINE_ELEMENT_HPP__
#define __GENERIC_PIPELINE_ELEMENT_HPP__
#include "IGenericPipelineElement.hpp"
#include "../Logging/ILogging.hpp"
#include <memory>
#include <string>
#include <mutex>
#include <queue>
#include <atomic>
#include <thread>
#include <condition_variable>

namespace FIPP
{
    namespace pipe
    {
        class GenericPipelineElement : public IGenericPipelineElement
        {
        public:
            GenericPipelineElement(std::string elemName, int elemId, std::shared_ptr<FIPP::logging::ILogger> log);
            ~GenericPipelineElement();
            virtual void connectPredecessor(int elemId) = 0;
            virtual void connectSuccessor(std::shared_ptr<IGenericPipelineElement> elem) = 0;
            virtual bool startElement(img::ImageContainerConfig imgConfig, int predecessorId) = 0;
            virtual bool stopElement() = 0;
            inline bool isRunning() { return this->m_isRunning; };

            void addImageToInputPipe(std::shared_ptr<img::ImageContainer> img);
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
             * @brief queue holding all incomming images for further processing
             *
             */
            std::queue<std::shared_ptr<img::ImageContainer>> m_inputQueue;
            /**
             * @brief Condition variable to wait worker thread
             *
             */
            std::condition_variable m_cvNotify;
            /**
             * @brief Variable to stop running worker loop.
             *
             */
            std::atomic_bool m_stop;
            /**
             * @brief stores if pipeline element is running
             *
             */
            bool m_isRunning;
            /**
             * @brief Thread to run the doCalculation function.
             *
             */
            std::thread m_workerThread;
            /**
             * @brief Send image out to next pipeline Element/Elements (depends on implementation)
             *
             * @param img shared ptr to outgoing image
             *
             */
            virtual void sendImageToSucessors(std::shared_ptr<img::ImageContainer> img) = 0;
            /**
             * @brief Overwrite by derived plugin to do calculation on next image
             *
             * @param img
             */
            virtual std::shared_ptr<img::ImageContainer> doCalculation(std::shared_ptr<img::ImageContainer> img) = 0;
            /**
             * @brief Start internal thread by startElement
             *
             */
            void startThread();

            /**
             * @brief Stop running thread.
             *
             * @return true
             * @return false
             */
            bool stopThread();

        private:
            void run();
            bool m_newImgArrived;
        };

    };
};

#endif