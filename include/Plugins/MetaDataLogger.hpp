#ifndef __RGB2GRAY_CONVERTER_HPP__
#define __RGB2GRAY_CONVERTER_HPP__
#include "../PipelineElements/GenericPipelineElementSiSo.hpp"
#include <yaml-cpp/yaml.h>

namespace FIPP
{
    namespace plugins
    {
        class MetaDataLogger : public pipe::GenericPipelineElementSiSo
        {
            public:
                MetaDataLogger(YAML::Node config, std::string elemName, int elemId, std::shared_ptr<FIPP::logging::ILogger> log);

            protected:
                std::shared_ptr<img::ImageContainer> doCalculation(std::shared_ptr<img::ImageContainer> img);
        };
    }
}
#endif