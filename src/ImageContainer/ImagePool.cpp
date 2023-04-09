#include "ImagePool.hpp"
#include "ImageContainerCPU.hpp"
#include "ImageContainerCUDA.hpp"

using namespace FIPP::img;

ImagePool::ImagePool(int size, ImageContainerConfig config)
{
    m_config = config;
    m_actualNumberOfContainers = 0;
    createImageContainers(size);
}

void ImagePool::addMoreImages(int size)
{
    createImageContainers(size);
}

void ImagePool::createImageContainers(int size)
{
    this->m_actualNumberOfContainers += size;
    if (m_config.backend.type == BackendType::CPU)
    {
        for (int i = 0; i < size; i++)
        {
            this->m_images.push_back(std::make_shared<ImageContainerCPU>(m_config.dimensions, m_config.imgFormat, static_cast<unsigned int>(i)));
        }
        int internalSize = this->m_images.size();
    }
    else if (m_config.backend.type == BackendType::CUDA)
    {
        for (int i = 0; i < size; i++)
        {
            this->m_images.push_back(std::make_shared<ImageContainerCUDA>(m_config.dimensions, m_config.imgFormat, m_config.backend.flags, static_cast<unsigned int>(i)));
        }
        int internalSize = this->m_images.size();
    }
    else
    {
        this->m_actualNumberOfContainers -= size;
    }
}

std::shared_ptr<IImageContainer> ImagePool::getNextFreeImage()
{
    for (auto img : this->m_images)
    {
        if (!img->isBound())
        {
            img->setBound();
            return img;
        }
    }
    throw NoFreeImageAvailableException();
}