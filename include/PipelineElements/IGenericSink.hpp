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

        /**
         * @brief Definition for Plugin Types for input/output configuration identification.
         * This information is hardcode in Generic classes.
         * 
         */
        typedef enum e_PluginTypes
        {
            SISO, ///< Single Input, Single Output
            MISO, ///< Multiple Input, Single Outptut
            SIMO, ///< Single Input, Multiple Output
            MIMO ///< Multiple Input, Multiple Output
        } PluginTypes;

        /**
         * @brief Interface for all Sinks.
         * 
         */
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
            /**
             * @brief Get the plugin/sink input/output configuration type.
             * Sinks can have SISO or MISO.
             * Plugins can have SISO, MISO, SIMO, MIMO
             * 
             * @return PluginTypes 
             */
            virtual PluginTypes getPluginType() const = 0;
        };

    };
};

#endif