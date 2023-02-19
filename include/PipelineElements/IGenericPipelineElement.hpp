/**
 * @file IGenericPipelineElement.hpp
 * @author Alexander Mueller (dev@alexandermaxmueller.de)
 * @brief Generic Interface for all Pipeline Elements
 * @version 0.2
 * @date 2022-10-13
 *
 * @copyright Copyright (c) 2022
 *
 * Interface for all PipelineElements independend of their implementation.
 * 
 * v0.1:
 * - implemented basics
 * - no elementState only running or not
 *
 */
#ifndef __IGENERIC_PIPELINE_ELEMENT_HPP__
#define __IGENERIC_PIPELINE_ELEMENT_HPP__

#include <memory>
#include <string>
#include "../ImageContainer/ImageFormat.hpp"
#include "../ImageContainer/ImageContainer.hpp"
namespace YAML{
    class Node;
}
namespace FIPP
{
    namespace logging{
        class ILogger;
    }

    namespace pipe
    {
        /**
         * @brief defines the type of implementation of the plugin
         *
         */
        typedef enum e_ElementTypes
        {
            SOURCE, ///< Identifies element as a Pipeline Source (output only)
            SINK, ///< Identifies element as a Pipeline Sink (input only)
            PLUGIN ///< Identifies element as a Pipeline Plugin (input/output)
        } ElementTypes;

        /**
         * @brief Internal state of every pipeline element.
         *
         */
        typedef enum e_elementState
        {
            IDLE,
            STARTING,
            RUNNING,
            STOPPING,
            PAUSED
        } ElementState;

        typedef enum e_startState{
            STARTED,
            PENDING,
            WRONG_PREDECESSOR,
            START_ERROR,
            START_WITHOUT_SUCESSOR
        } StartState;

        typedef enum e_stopState{
            STOPPED,
            STOP_ERROR,
            ALREADY_STOPPED
        } StopState;

        class IGenericPipelineElement
        {
        public:
            /**
             * @brief Start element thread
             * 
             * @param predecessorId element id of the pipe element calling this function (-1 if calling function is a supervisor e.g. for sinks)
             * @return true 
             * @return false 
             */
            virtual StartState startElement(int predecessorId) = 0;
            virtual StopState stopElement() = 0;
            /**
             * Get current state from internal state machine.
            */
            virtual ElementState getState() = 0;
            /**
             * @brief Get name of Pipeline Element (VideoCamera_RearView)
             * 
             * @return pipelineElement name
            */
            virtual std::string getName() const = 0;
            virtual int getId() const = 0;
            virtual ElementTypes getElementType() = 0;
        };
        
        class IGenericPipelineElementFactory
        {
            public:
                /**
                 * @brief creates a pipeline element and returns a shared ptr on this object.
                 * @return shared_ptr of Type IGenericPipelineElement castable to @see GenericSource, @see GenericSink, @see GenericPlugin
                */
                virtual std::shared_ptr<IGenericPipelineElement> makePipeElement(YAML::Node config, std::string elementName, int elemId, std::shared_ptr<FIPP::logging::ILogger> log) = 0;
                /**
                 * @brief get the corresponding element identifier of the plugin created by these factory. In general the name of this plugin.
                 * @return std::string name of plugin
                */
                virtual std::string getElementIdentifier() = 0;
                /**
                 * @brief Returns the ElementType (SINK, SOURCE, PLUGIN) @see ElementTypes
                 * @return ElementType of the created pipelineElement;
                */
                virtual ElementTypes getElementType() = 0;
        };
    };
};

#endif