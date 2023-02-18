/**
 * @file IGenericSink.hpp
 * @author Alexander Mueller (dev@alexandermaxmueller.de)
 * @brief An interface for all pipeline sinks.
 * @version 0.1
 * @date 2023-01-25
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef __IGENERIC__SINK_HPP__
#define __IGENERIC__SINK_HPP__

#include <memory>
#include <string>
#include "IGenericPipelineElement.hpp"
#include "../ImageContainer/ImageFormat.hpp"
#include "../ImageContainer/ImageContainer.hpp"

#define INPUT_FILTER_ALLOW_ALL_INPUTS false
#define INPUT_FILTER_CHECK_VALUES true
namespace FIPP
{

    namespace pipe
    {
        class IGenericSink : public IGenericPipelineElement
        {
        public:
            /**
             * @brief set a element identified by elem id as predecessor in pipeline structure.
             * 
             * @param elemId 
             */
            virtual void connectPredecessor(int elemId) = 0;
            /**
             * @brief Add an image to input pipeline.
             * 
             * @param img 
             */
            virtual void addImageToInputPipe(std::shared_ptr<img::ImageContainer> img) = 0;
            /**
             * @brief Check if the incomming images can be processed by this pipeline element.
             * 
             * @param imgConfig configuration of the output image format of the predecessor
             * @param predecessorId id of the predecessor to check if this is a valid predeccessor.
             * @return true 
             * @return false 
             */
            virtual bool interogateConnection(img::ImageContainerConfig imgConfig, int predecessorId) = 0;

        };

    };
};

#endif