#include <gtest/gtest.h>
#include <memory>
#include <yaml-cpp/yaml.h>
#include <exception>
#include "../../include/Point.hpp"
#include "ImageContainer/IImageContainer.hpp"
#include "ImageContainer/ImageFormat.hpp"
#include "ImageContainer/IImagePool.hpp"
#include "ImageContainer/ImagePoolFactory.hpp"

using namespace FIPP;
// Demonstrate some basic assertions.
TEST(Creation, basics)
{
    // Expect equality.
    Point<unsigned int> size(10, 10);
    img::ImageContainerConfig config;
    config.backend.type = img::BackendType::CPU;
    config.dimensions = size;
    config.imgFormat.bitDepthPerPixel=8;
    config.imgFormat.bytesPerPixel = 1;
    config.imgFormat.imgType = img::ImageType::GRAY;
    std::unique_ptr<img::IImagePool> pPool = img::getImagePool(1, config);
    std::shared_ptr<img::IImageContainer> pImg = pPool->getNextFreeImage();
    EXPECT_EQ(pImg->getBackendType(), img::BackendType::CPU);
    EXPECT_EQ(pImg->getBackendFlags(), 0);
    EXPECT_EQ(pImg->getDims().getArea(), 100);
    EXPECT_EQ(pImg->getImgFormat().bitDepthPerPixel, 8);
    EXPECT_EQ(pImg->getImgFormat().bytesPerPixel, 1);
}

TEST(Creation, bounding)
{
    // Expect equality.
    Point<unsigned int> size(10, 10);
    img::ImageContainerConfig config;
    config.backend.type = img::BackendType::CPU;
    config.dimensions = size;
    config.imgFormat.bitDepthPerPixel=8;
    config.imgFormat.bytesPerPixel = 1;
    config.imgFormat.imgType = img::ImageType::GRAY;
    std::unique_ptr<img::IImagePool> pPool = img::getImagePool(1, config);
    std::shared_ptr<img::IImageContainer> pImg = pPool->getNextFreeImage();
    pImg->setUnBound();
    EXPECT_EQ(pImg->isBound(), false);
    pImg->setBound();
    EXPECT_EQ(pImg->isBound(), true);
    pImg->setUnBound();
    EXPECT_EQ(pImg->isBound(), false);
}

TEST(ImageFormat, fromYAML){
    YAML::Node node;
    YAML::Node dims;
    dims["x"] = 100;
    dims["y"] = 200;
    node["dimensions"] = dims;
    node["bitDepthPerPixel"] = 8;
    node["imgType"] = "GRAY";
    node["bytesPerPixel"] = 1;
    node["backendType"] = "CPU";
    
    img::ImageContainerConfig conf;
    conf.imgFormat.bitDepthPerPixel = 8;
    conf.imgFormat.bytesPerPixel = 1;
    conf.imgFormat.imgType = img::ImageType::GRAY;
    conf.dimensions = Point<unsigned int>(100, 200);
    conf.backend.type = img::BackendType::CPU;
    
    img::ImageContainerConfig compConf = img::getContainerConfigFromYaml(node);
    
    EXPECT_EQ(compConf.dimensions.getX(), conf.dimensions.getX());
    EXPECT_EQ(compConf.dimensions.getY(), conf.dimensions.getY());
    EXPECT_EQ(compConf.backend.type, conf.backend.type);
    EXPECT_EQ(compConf.imgFormat.bitDepthPerPixel, conf.imgFormat.bitDepthPerPixel);
    EXPECT_EQ(compConf.imgFormat.bytesPerPixel, conf.imgFormat.bytesPerPixel);
    std::cout << conf.imgFormat.imgType << std::endl;
    EXPECT_EQ(compConf.imgFormat.imgType, conf.imgFormat.imgType);

}

TEST(Pooling, basics){
    img::ImageContainerConfig conf;
    conf.backend.flags = img::BackendFlags::CPU_ONLY;
    conf.backend.type = img::BackendType::CPU;
    conf.dimensions = Point<unsigned int>(100,100);
    img::ImageFormat format;
    format.bitDepthPerPixel=8;
    format.bytesPerPixel = 1;
    format.imgType = img::ImageType::GRAY;
    conf.imgFormat = format;
    std::unique_ptr<img::IImagePool> pPool = img::getImagePool(10, conf);
    EXPECT_EQ(pPool->getNumberOfContainers(), 10);
    std::vector<std::shared_ptr<img::IImageContainer>> imgs;
    for(int i = 0; i < 11; i++){
        if(i < 10){
            std::shared_ptr<img::IImageContainer> img;
            EXPECT_NO_THROW(img = pPool->getNextFreeImage());
            imgs.push_back(img);
            EXPECT_EQ(img->getUUID(), i);
        }else{
            EXPECT_THROW(std::shared_ptr<img::IImageContainer> img = pPool->getNextFreeImage(), img::NoFreeImageAvailableException);
        }
    }
    for(auto e : imgs){
        e->setUnBound();
    }
    for(int i = 0; i < 11; i++){
        if(i < 10){
            std::shared_ptr<img::IImageContainer> img;
            EXPECT_NO_THROW(img = pPool->getNextFreeImage());
            imgs.push_back(img);
            EXPECT_EQ(img->getUUID(), i);
        }else{
            EXPECT_THROW(std::shared_ptr<img::IImageContainer> img = pPool->getNextFreeImage(), img::NoFreeImageAvailableException);
        }
    }
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}