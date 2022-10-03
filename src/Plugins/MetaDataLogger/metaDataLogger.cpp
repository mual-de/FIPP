#include "Plugins/MetaDataLogger.hpp"

using namespace FIPP::plugins;

MetaDataLogger::MetaDataLogger(YAML::Node config, std::string elemName, int elemId, std::shared_ptr<FIPP::logging::ILogger> log) : GenericPipelineElementSiSo(elementName, elemId, log){
    printf("test");
}


std::shared_ptr<img::ImageContainer> MetaDataLogger::doCalculation(std::shared_ptr<img::ImageContainer> img){
    return img;
}