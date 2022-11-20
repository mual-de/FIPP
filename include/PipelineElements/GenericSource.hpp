#ifndef __GENERIC__SOURCE_HPP__
#define __GENERIC__SOURCE_HPP__

#include <memory>
#include <string>
#include <thread>
#include <atomic>
#include "../Logging/ILogging.hpp"
#include "IGenericSource.hpp"
#include "IGenericSink.hpp"
#include "../ImageContainer/ImageFormat.hpp"
#include "../ImageContainer/ImageContainer.hpp"
#include "../ImageContainer/ImagePool.hpp"
namespace FIPP
{

    namespace pipe
    {
        class GenericSource : public IGenericSource
        {
        public:
            GenericSource(std::string elemName, int elemId, std::shared_ptr<FIPP::logging::ILogger> log);
            ~GenericSource();
            virtual bool startElement(int predecessorId) = 0;
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
            /**
             * @brief connect successor to this pipelineElement
            */
            void connectSuccessor(std::shared_ptr<IGenericSink> elem){
                this->m_successor = elem;
            }

            bool interogateConnection(){
                return m_successor->interogateConnection(this->m_sourceConfig, this->m_elemId);
            }

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
            /**
             * @brief successor connected to this source
            */
            std::shared_ptr<IGenericSink> m_successor;

            std::mutex m_inputLockMutex;
            /**
             * @brief Variable to stop running worker loop.
             *
             */
            std::atomic_bool m_stop;
            /**
             * @brief configuration of this src outgoing packages
            */
            img::ImageContainerConfig m_sourceConfig;

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

            float m_fps = 20.00f;
            std::chrono::milliseconds m_fps_duration;

            std::unique_ptr<img::ImagePool> m_pool;
            virtual void doCalculation(std::shared_ptr<img::ImageContainer> img) = 0;
            virtual void initializeInterfaces() = 0;
            virtual void closeInterfaces() = 0;
            void startThread();
            bool stopThread();

        private:
            void run();
        };
    };

};

#endif