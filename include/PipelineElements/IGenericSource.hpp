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

        class IGenericSink;
        class IGenericSource : public IGenericPipelineElement
        {
        public:
            virtual void connectSuccessor(std::shared_ptr<IGenericSink> elem) = 0;
            virtual bool interogateConnection() = 0;
        };

    };
};

#endif