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
namespace FIPP
{

    namespace pipe
    {
        /**
         * @brief defines the type of implementation of the plugin
         *
         */
        typedef enum e_ElementTypes
        {
            SOURCE,
            SINK,
            PLUGIN
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
            virtual bool startElement(int predecessorId) = 0;
            virtual bool stopElement() = 0;
            virtual ElementState getState() = 0;
            virtual std::string getName() const = 0;
            virtual int getId() const = 0;
            virtual ElementTypes getElementType() = 0;
        };

    };
};

#endif