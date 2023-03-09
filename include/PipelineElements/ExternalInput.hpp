#ifndef __EXTERNAL_INPUT_HPP__
#define __EXTERNAL_INPUT_HPP__
#include <memory>
#include "../Logging/ILogging.hpp"
#include "IGenericSource.hpp"
#include "ImageContainer/ImageFormat.hpp"
#include "ImageContainer/ImagePool.hpp"

namespace FIPP
{
    namespace pipe
    {

        class ExternalInput : public IGenericSource
        {

        public:
            ExternalInput(YAML::Node config, int elemId, std::shared_ptr<FIPP::logging::ILogger> log);
            ~ExternalInput();
            inline int getId() const { return m_uuid; };
            std::shared_ptr<img::ImageContainer> getImgContainer();
            void pushImgToPipe(std::shared_ptr<img::ImageContainer> img);
            void connectSuccessor(std::shared_ptr<pipe::IGenericSink> elem);
            bool interogateConnection();
            /**
             * @brief Start element thread
             *
             * @param predecessorId element id of the pipe element calling this function (-1 if calling function is a supervisor e.g. for sinks)
             * @return true
             * @return false
             */
            StartState startElement(int predecessorId);
            StopState stopElement();
            /**
             * Get current state from internal state machine.
             */
            ElementState getState() { return m_state; };
            /**
             * @brief Get name of Pipeline Element (VideoCamera_RearView)
             *
             * @return pipelineElement name
             */
            std::string getName() const { return m_name; };

            /**
             * @brief Get the type of this IGenericSource --> always SOURCE
             *
             * @return pipe::ElementTypes
             */
            inline pipe::ElementTypes getElementType() const { return pipe::ElementTypes::SOURCE; };

        private:
            /**
             * @brief Internal image pool to provide images for further processing
             *
             */
            std::unique_ptr<img::ImagePool> m_pool;
            /**
             * @brief unique identifier in pipeline
             * 
             */
            int m_uuid;
            /**
             * @brief name of this external input.
             *
             */
            std::string m_name;
            /**
             * @brief Pointer to next element in pipe.
             *
             */
            std::shared_ptr<pipe::IGenericSink> m_successor;
            /**
             * @brief Logger to log all relevant information
             * 
             */
            std::shared_ptr<FIPP::logging::ILogger> m_log;
            /**
             * @brief Current state of plugin
             *
             */
            ElementState m_state;
            img::ImageContainerConfig m_config;
        };
    }
}

#endif