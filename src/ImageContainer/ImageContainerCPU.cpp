#include "ImageContainer/ImageContainerCPU.hpp"
#include <cstring>
using namespace FIPP::img;

ImageContainerCPU::ImageContainerCPU(Point<unsigned int> size, ImageFormat format) : ImageContainer(size, format)
{
    m_data = (unsigned char *)malloc(m_memsize);
}

ImageContainerCPU::~ImageContainerCPU()
{
    free(m_data);
}

const unsigned char *ImageContainerCPU::getConstPtr() const
{
    return m_data;
};
unsigned char *ImageContainerCPU::getPtr() const
{
    return m_data;
};
ContainerError ImageContainerCPU::updateMemory(unsigned long long int frame, const unsigned char *data, int size, Backend backend, int memPitch)
{
    this->m_frameNumber = frame;
    if (backend.type == BackendType::CPU)
    {
        if (size != this->m_memsize)
        {
            return ContainerError::INVALID_SIZE;
        }
        if (memPitch == 0)
        {
            std::memcpy(m_data, data, size);
            return ContainerError::OKAY;
        }
    }
    return ContainerError::INVALID_BACKEND;
}


ContainerError ImageContainerCPU::updateMemory(std::shared_ptr<ImageContainer> img)
{
    this->m_frameNumber = img->getFrameNumber();
    if(img->getBackendType() != BackendType::CPU){
        return ContainerError::INVALID_SIZE;
    }
    if(img->getImgFormat() != m_imgFormat){
        return ContainerError::INVALID_FORMAT;
    }
    if(img->getDims() != this->m_dims){
        return ContainerError::INVALID_SIZE;
    }
    std::memcpy(this->m_data, img->getConstPtr(), this->m_memsize);
    return ContainerError::OKAY;
}