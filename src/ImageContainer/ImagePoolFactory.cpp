#include "ImageContainer/IImagePool.hpp"
#include "ImageContainer/ImagePoolFactory.hpp"
#include "ImagePool.hpp"
using namespace FIPP::img;
std::unique_ptr<IImagePool> FIPP::img::getImagePool(int size, ImageContainerConfig config){
    return std::make_unique<ImagePool>(size, config);
}