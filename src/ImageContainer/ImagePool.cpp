#include "ImageContainer/ImagePool.hpp"
#include "ImageContainer/ImageContainerCPU.hpp"
using namespace FIPP::img;


ImagePool::ImagePool(int size, ImageContainerConfig config)
{
    m_config = config;
    m_actualNumberOfContainers = 0;
    createImageContainers(size);
}

void ImagePool::addMoreImages(int size){
    createImageContainers(size);
}

void ImagePool::createImageContainers(int size){
    this->m_actualNumberOfContainers += size;
    if(m_config.backend.type == BackendType::CPU){
        for(int i = 0; i < size; i++){
            this->m_images.push_back(std::make_shared<ImageContainerCPU>(m_config.dimensions, m_config.imgFormat, static_cast<unsigned int>(i)));
        }
        int internalSize = this->m_images.size();
    }else{
        this->m_actualNumberOfContainers -= size;
    }
}

std::shared_ptr<ImageContainer> ImagePool::getNextFreeImage(){
    for(auto img : this->m_images){
        if(!img->isBound()){
            img->setBound();
            return img;
        }
    }
    return std::shared_ptr<ImageContainer>(nullptr);
}