#ifndef __IIMAGE_POOL_HPP__
#define __IIMAGE_POOL_HPP__

#include "ImageFormat.hpp"
#include "IImageContainer.hpp"
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
        class IImagePool
        {
        public:
            /**
             * @brief Get the Next Free Image object
             * Returns a shared pointer to an ImageContainter object which is currently not in use by the pipeline.
             * Surround with try/catch to solve issues if no free ImageContainer is available.
             * @return std::shared_ptr<ImageContainer>
             */
            virtual std::shared_ptr<IImageContainer> getNextFreeImage() = 0;
            /**
             * @brief Create more ImageContainers and add them to internal vector.
             *
             * @param size number of ImageContainers to be added
             */
            virtual void addMoreImages(int size) = 0;
            /**
             * @brief Get the number of total ImageContainers supervised by this ImagePool
             *
             * @return int number of available ImageContainers
             */
            virtual int getNumberOfContainers() const = 0;
        };
    }
}

#endif