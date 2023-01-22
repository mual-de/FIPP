#ifndef __IMAGE_CONTAINER_CUDA_HPP__
#define __IMAGE_CONTAINER_CUDA_HPP__

#ifndef __IMAGE_CONTAINER_CPU_HPP__
#define __IMAGE_CONTAINER_CPU_HPP__

#include "ImageContainer.hpp"
#include "../Point.hpp"
#include "ImageFormat.hpp"

namespace FIPP
{
    namespace img
    {
        class ImageContainerCUDA : public ImageContainer
        {

        public:
            ImageContainerCPU(Point<unsigned int> size, ImageFormat format, unsigned int uuid);
            ~ImageContainerCPU();
            const unsigned char *getConstPtr() const;
            unsigned char *getPtr() const;
            const unsigned char *getConstDevPtr() const;
            unsigned char *getDevPtr() const;
            ContainerError updateMemory(unsigned long long int frame, const unsigned char *data, int size, Backend backend, int memPitch = 0);
            ContainerError updateMemory(std::shared_ptr<ImageContainer> img);
            bool allowsCPUAccess(){return this->m_allowsCPUAccess;};

        private:
            unsigned char *m_data;
            bool m_allowsCPUAccess;
        };
    }
}
#endif
#endif