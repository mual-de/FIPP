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
            virtual void connectPredecessor(int elemId) = 0;
            virtual void addImageToInputPipe(std::shared_ptr<img::ImageContainer> img) = 0;
            /**
             * @brief Check if the incomming images can be processed by this pipeline element.
             * 
             * @param imgConfig 
             * @param predecessorId 
             * @return true 
             * @return false 
             */
            virtual bool interogateConnection(img::ImageContainerConfig imgConfig, int predecessorId) = 0;

        };

    };
};

#endif