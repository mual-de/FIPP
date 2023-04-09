/**
 * @file GenericPlugin.hpp
 * @author Alexander Mueller (dev@alexandermaxmueller.de)
 * @brief Implements a GenericPlugin
 * @version 0.1
 * @date 2022-10-13
 * 
 * @copyright Copyright (c) 2022
 * 
 * Basic class for all plugins, provides general functions for threading and
 * startup phase.
 * 
 */
#ifndef __GENERIC_PLUGIN_ELEMENT_HPP__
#define __GENERIC_PLUGIN_ELEMENT_HPP__
#include "IGenericPlugin.hpp"
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
        class GenericPlugin : public IGenericPlugin
        {
        public:
            GenericPlugin(std::string elemName, int elemId, std::shared_ptr<FIPP::logging::ILogger> log);
            ~GenericPlugin();
            inline ElementState getState() { return this->m_state; };
            void addImageToInputPipe(std::shared_ptr<img::IImageContainer> img);
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
            inline ElementTypes getElementType() const { return ElementTypes::PLUGIN; };
            /**
             * @brief Run internal doCalculation function for software tests.
             * 
             * @param img 
             * @return std::shared_ptr<img::IImageContainer> 
             */
            inline std::shared_ptr<img::IImageContainer> testInternalFunction(std::shared_ptr<img::IImageContainer> img){return this->doCalculation(img);};

            

        protected:
            const std::string m_elemName;
            const int m_elemId;
            bool m_filterActivated;
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
            std::queue<std::shared_ptr<img::IImageContainer>> m_inputQueue;
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
             * @brief Current state of plugin
             *
             */
            ElementState m_state;
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
            virtual void sendImageToSucessors(std::shared_ptr<img::IImageContainer> img) = 0;
            /**
             * @brief Overwrite by derived plugin to do calculation on next image
             *
             * @param img
             */
            virtual std::shared_ptr<img::IImageContainer> doCalculation(std::shared_ptr<img::IImageContainer> img) = 0;
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