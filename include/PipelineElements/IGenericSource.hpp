#ifndef __IGENERIC__SOURCE_HPP__
#define __IGENERIC__SOURCE_HPP__

#include <memory>
#include <string>
#include "IGenericPipelineElement.hpp"
#include "../ImageContainer/ImageFormat.hpp"
#include "../ImageContainer/ImageContainer.hpp"
namespace FIPP
{

    namespace pipe
    {
        class IGenericSource : public IGenericPipelineElement
        {
        public:
            virtual bool startElement(img::ImageContainerConfig imgConfig, int predecessorId) = 0;
            virtual bool stopElement() = 0;
            virtual bool isRunning() = 0;
            virtual void connectSuccessor(std::shared_ptr<IGenericPipelineElement> elem) = 0;
            virtual std::string getName() const = 0;
            virtual int getId() const = 0;

        };

    };
};

#endif