#include <gtest/gtest.h>
#include <memory>
#include "Point.hpp"
#include "ImageContainer/ImageContainer.hpp"
#include "ImageContainer/ImageContainerCPU.hpp"
#include "ImageContainer/ImageFormat.hpp"

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
    std::shared_ptr<img::ImageContainer> pImg = std::make_shared<img::ImageContainerCPU>(size, format);
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
    std::shared_ptr<img::ImageContainer> pImg = std::make_shared<img::ImageContainerCPU>(size, format);
    EXPECT_EQ(pImg->isBound(), false);
    pImg->setBound();
    EXPECT_EQ(pImg->isBound(), true);
    pImg->setUnBound();
    EXPECT_EQ(pImg->isBound(), false);
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}