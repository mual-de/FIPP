/**
 * @file ImagePoolFactory.hpp
 * @author Alexander Mueller (dev@alexandermaxmueller.de)
 * @brief A factory for image pools
 * @version 0.1
 * @date 2023-04-09
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef __IMAGE_POOL_FACTORY_HPP__
#define __IMAGE_POOL_FACTORY_HPP__
#include <memory>
#include "ImageFormat.hpp"

namespace FIPP
{
    namespace img{
        class IImagePool;
        std::unique_ptr<IImagePool> getImagePool(int size, ImageContainerConfig config);
    }
}

#endif