#include <gtest/gtest.h>
#include <memory>
#include "Point.hpp"
#include "ImageContainer/ImageContainer.hpp"
#include "ImageContainer/ImageContainerCPU.hpp"
#include "ImageContainer/ImageFormat.hpp"
#include "ImageContainer/ImagePool.hpp"

using namespace FIPP;
// Demonstrate some basic assertions.
TEST(Creation, basics)
{
    // Expect equality.
    Point<unsigned int> size(10, 10);
    img::ImageFormat format;
    format.bitDepthPerPixel=8;
    format.bytesPerPixel = 1;
    format.imgType = img::ImageType::GRAY;
    std::shared_ptr<img::ImageContainer> pImg = std::make_shared<img::ImageContainerCPU>(size, format,0);
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
    img::ImageFormat format;
    format.bitDepthPerPixel=8;
    format.bytesPerPixel = 1;
    format.imgType = img::ImageType::GRAY;
    std::shared_ptr<img::ImageContainer> pImg = std::make_shared<img::ImageContainerCPU>(size, format,0);
    EXPECT_EQ(pImg->isBound(), false);
    pImg->setBound();
    EXPECT_EQ(pImg->isBound(), true);
    pImg->setUnBound();
    EXPECT_EQ(pImg->isBound(), false);
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
    std::unique_ptr<img::ImagePool> pool = std::make_unique<img::ImagePool>(10, conf);
    EXPECT_EQ(pool->getNumberOfContainers(), 10);
    std::vector<std::shared_ptr<img::ImageContainer>> imgs;
    for(int i = 0; i < 11; i++){
        if(i < 10){
            std::shared_ptr<img::ImageContainer> img = pool->getNextFreeImage();
            imgs.push_back(img);
            EXPECT_EQ(img->getUUID(), i);
        }else{
            EXPECT_EQ(pool->getNextFreeImage().get(), nullptr);
        }
    }
    for(auto e : imgs){
        e->setUnBound();
    }
    for(int i = 0; i < 11; i++){
        if(i < 10){
            std::shared_ptr<img::ImageContainer> img = pool->getNextFreeImage();
            imgs.push_back(img);
            EXPECT_EQ(img->getUUID(), i);
        }else{
            EXPECT_EQ(pool->getNextFreeImage(), nullptr);
        }
    }
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}