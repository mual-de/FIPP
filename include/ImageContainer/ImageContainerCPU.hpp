/**
 * @file ImageContainerCPU.hpp
 * @author Alexander Mueller (dev@alexandermaxmueller.de)
 * @brief Implementation of a host memory ImageContainer
 * @version 0.1
 * @date 2023-03-09
 * 
 * @copyright Copyright (c) 2023
 * 
 */
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
            ImageContainerCPU(Point<unsigned int> size, ImageFormat format, unsigned int uuid);
            ~ImageContainerCPU();
            /**
             * @brief Get a const pointer to allocated memory
             * 
             * @return const unsigned char* 
             */
            const unsigned char *getConstPtr() const;
            /**
             * @brief Get a pointer to allocated memory
             * Use the lock provided by ImageContainer if you want to write to this container!
             * 
             * @return unsigned char* 
             */
            unsigned char *getPtr() const;
            ContainerError updateMemory(unsigned long long int frame, const unsigned char *data, Point<unsigned int> dims, int bytesPerPixel, Backend backend, int memPitch);
            ContainerError updateMemory(std::shared_ptr<ImageContainer> img);

        private:
            unsigned char *m_data;
        };
    }
}
#endif