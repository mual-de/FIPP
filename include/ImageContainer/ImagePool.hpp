/**
 * @file ImagePool.hpp
 * @author Alexander Mueller (dev@alexandermaxmueller.de)
 * @brief Implementation of an ImagePool to provide access to ImageContainers preallocated before pipeline starts.
 * @version 0.1
 * @date 2023-01-22
 *
 * @copyright Copyright (c) 2023
 *
 */
#ifndef __IMAGE_POOL_HPP__
#define __IMAGE_POOL_HPP__
#include "ImageFormat.hpp"
#include "ImageContainer.hpp"
#include <vector>

namespace FIPP
{
    namespace img
    {
        /**
         * @brief Exception if ImagePool has no free image available.
         *
         */
        struct NoFreeImageAvailableException : public std::exception
        {
            const char *what() const throw()
            {
                return "No free image available in pool";
            }
        };

        /**
         * @brief ImagePool is a supervisor for ImageContainers.
         *
         */
        class ImagePool
        {
        public:
            ImagePool(int size, ImageContainerConfig config);
            /**
             * @brief Get the Next Free Image object
             * Returns a shared pointer to an ImageContainter object which is currently not in use by the pipeline.
             * Surround with try/catch to solve issues if no free ImageContainer is available.
             * @return std::shared_ptr<ImageContainer>
             */
            std::shared_ptr<ImageContainer> getNextFreeImage();
            /**
             * @brief Create more ImageContainers and add them to internal vector.
             *
             * @param size number of ImageContainers to be added
             */
            void addMoreImages(int size);
            /**
             * @brief Get the number of total ImageContainers supervised by this ImagePool
             *
             * @return int number of available ImageContainers
             */
            inline int getNumberOfContainers() const { return m_actualNumberOfContainers; };

        private:
            /**
             * @brief Configuration for ImageContainers
             *
             */
            ImageContainerConfig m_config;
            /**
             * @brief Number of ImageContainers created by this pool.
             *
             */
            int m_actualNumberOfContainers;
            /**
             * @brief vector of all created ImageContainers.
             *
             */
            std::vector<std::shared_ptr<ImageContainer>> m_images;
            /**
             * @brief Create a number of ImageContainers and add them to m_images.
             *
             * @param size
             */
            void createImageContainers(int size);
        };
    }
}

#endif