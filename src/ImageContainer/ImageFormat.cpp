#include "ImageContainer/ImageFormat.hpp"
#include <yaml-cpp/yaml.h>

FIPP::img::ImageContainerConfig FIPP::img::getContainerConfigFromYaml(YAML::Node containerNode)
{
    ImageContainerConfig conf;
    if (!containerNode[ "dimensions"])
    {
        throw std::invalid_argument("Dimension is not set in YAML file!");
    }
    conf.dimensions = FIPP::Point<int>(containerNode["dimensions"]);
    if (!containerNode["bitDepthPerPixel"])
    {
        throw std::invalid_argument("bitDepthPerPixel is not set in YAML file!");
    }
    conf.imgFormat.bitDepthPerPixel = containerNode["bitDepthPerPixel"].as<unsigned char>();
    if (!containerNode["bytesPerPixel"])
    {
        throw std::invalid_argument("bytesPerPixel is not set in YAML file!");
    }
    if(containerNode["usePitch"]){
        conf.imgFormat.usePitch = containerNode["usePitch"].as<bool>();
    }else{
        conf.imgFormat.usePitch = false;
    }
    if(containerNode["backendFlags"]){
        conf.backend.flags = getBackendFlags(containerNode["backendFlags"].as<std::string>());
    }
    conf.imgFormat.bytesPerPixel = containerNode["bytesPerPixel"].as<unsigned char>();
    conf.imgFormat.imgType = getImageType(containerNode["imgType"].as<std::string>());
    conf.backend.type = getBackendType(containerNode["backendType"].as<std::string>());
    return conf;
};