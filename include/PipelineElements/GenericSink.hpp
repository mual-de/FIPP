/**
 * @file GenericSink.hpp
 * @author alexander Mueller (dev@alexandermaxmueller.de)
 * @brief Generic Class for a Pipeline Element with one input and no ouptu (end of pipeline, e.g. connection to gstreamer)
 * @version 0.1
 * @date 2022-10-08
 * 
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef __GENERIC_SINK_HPP__
#define __GENERIC_SINK_HPP__
#include "IGenericSink.hpp"
#include "InputFilter.hpp"
#include "../Logging/ILogging.hpp"
#include <memory>
#include <string>
#include <mutex>
#include <queue>
#include <thread>
#include <condition_variable>

namespace FIPP
{
    namespace pipe
    {
        class GenericSink : public IGenericSink
        {
        public:
            GenericSink(std::string elemName, int elemId, std::shared_ptr<FIPP::logging::ILogger> log);
            ~GenericSink();
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

            inline ElementTypes getElementType(){return ElementTypes::SINK;};
            inline ElementState getState(){return this->m_state;};
            virtual bool interogateConnection(img::ImageContainerConfig imgConfig, int predecessorId) = 0;


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
            bool m_stop;
            /**
             * Current state of these pipeline element
             * @see pipe::ElementState
            */
            ElementState m_state;
            /**
             * @brief Thread to run the doCalculation function.
             * 
             */
            std::thread m_workerThread;
            bool m_filterActivated;
            /**
             * @brief Filter for interogation requests to check if these can be handled by this sink.
             * 
             */
            InputFilter m_filter;
            /**
             * @brief Overwrite by derived plugin to do calculation on next image
             * 
             * @param img 
             */
            virtual void doCalculation(std::shared_ptr<img::ImageContainer> img) = 0;
            virtual void initializeInterfaces() = 0;
            virtual void closeInterfaces() = 0;
            void startThread();
            bool stopThread();
        private:
            void run();
            bool m_newImgArrived;
        };

    };
};

#endif