#ifndef __IMAGE_POOL_HPP__
#define __IMAGE_POOL_HPP__
#include "ImageFormat.hpp"
#include "ImageContainer.hpp"
#include <vector>

namespace FIPP{
    namespace img{
        class ImagePool{
            public:
                ImagePool(int size, ImageContainerConfig config);
                std::shared_ptr<ImageContainer> getNextFreeImage();
                void addMoreImages(int size);
                inline int getNumberOfContainers() const {return m_actualNumberOfContainers;};
            private:
                ImageContainerConfig m_config;
                int m_actualNumberOfContainers;
                std::vector<std::shared_ptr<ImageContainer>> m_images;
                void createImageContainers(int size);
        };
    }
}

#endif