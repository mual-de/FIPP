#include "Plugins/MetaDataLogger.hpp"
#include <string.h>
using namespace FIPP;
using namespace FIPP::plugins;
using namespace FIPP::logging;

#define LOG(lvl, msg) this->m_log->log(lvl, msg, "MetaDataLogger", __func__, __LINE__, this->m_frameNumber);

MetaDataLogger::MetaDataLogger(YAML::Node config, int elemId, std::shared_ptr<FIPP::logging::ILogger> log) : GenericPipelineElementSiSo("MetaDataLogger", elemId, log){
    LOG(LogLevel::INFO, "Plugin created");
}
MetaDataLogger::~MetaDataLogger(){
    LOG(LogLevel::INFO, "Plugin shutdown");
}

std::shared_ptr<img::ImageContainer> MetaDataLogger::doCalculation(std::shared_ptr<img::ImageContainer> img){
    LOG(LogLevel::INFO, "Dims: " + img->getDims().asStr());
    LOG(LogLevel::INFO, "BackendType: " + std::to_string(img->getBackendType()));
    return img;
}