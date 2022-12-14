#include "ImageContainer/ImageContainer.hpp"

using namespace FIPP::img;

ImageContainer::ImageContainer(Point<unsigned int> size, ImageFormat fmt, unsigned int uuid): m_dims(size), m_frameNumber(0), m_imgFormat(fmt), m_uuid(uuid){
    this->m_memsize = size.getArea() * fmt.bytesPerPixel * (fmt.bitDepthPerPixel % 8 + 1);
    this->m_activeBound = false;
    this->m_mutexPtr = std::make_shared<std::mutex>();
}