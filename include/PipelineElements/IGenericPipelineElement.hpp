/**
 * @file IGenericPipelineElement.hpp
 * @author Alexander Mueller (dev@alexandermaxmueller.de)
 * @brief Generic Interface for all Pipeline Elements
 * @version 0.1
 * @date 2022-09-24
 * 
 * @copyright Copyright (c) 2022
 * 
 * Interface for all PipelineElements independend of their implementation.
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
        typedef enum e_ElementTypes
        {
            SOURCE,
            SINK,
            PLUGIN
        } ElementTypes;

        class IGenericPipelineElement
        {
        public:
            virtual bool startElement(img::ImageContainerConfig imgConfig, int predecessorId) = 0;
            virtual bool stopElement() = 0;
            virtual bool isRunning() = 0;
            virtual std::string getName() const = 0;
            virtual int getId() const = 0;
            virtual ElementTypes getElementType() = 0;

        };

    };
};

#endif