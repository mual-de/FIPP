#ifndef __IMAGE_CONTAINER_CPU_HPP__
#define __IMAGE_CONTAINER_CPU_HPP__

#include "ImageContainer.hpp"
#include "../Point.hpp"
#include "ImageFormat.hpp"

namespace FIPP
{
    namespace img
    {
        class ImageContainerCPU : public ImageContainer
        {

        public:
            ImageContainerCPU(Point<unsigned int> size, ImageFormat format);
            ~ImageContainerCPU();
            const unsigned char *getConstPtr() const;
            unsigned char *getPtr() const;
            ContainerError updateMemory(unsigned long long int frame, const unsigned char *data, int size, Backend backend, int memPitch = 0);
            ContainerError updateMemory(std::shared_ptr<ImageContainer> img);

        private:
            unsigned char *m_data;
        };
    }
}
#endif