#ifndef __IMAGE_CONTAINER_HPP__
#define __IMAGE_CONTAINER_HPP__

#include "ImageFormat.hpp"
#include "../Point.hpp"

#include <mutex>
#include <memory>

namespace FIPP
{
    namespace img
    {
        typedef enum e_containerError
        {
            OKAY,
            UNKNOWN_ERROR,
            INVALID_BACKEND,
            INVALID_SIZE,
            INVALID_FORMAT
        } ContainerError;

        class ImageContainer
        {
        protected:
            Point<unsigned int> m_dims;
            /**
             * @brief Actual frame number if img in this container.
             *
             */
            unsigned long long int m_frameNumber;
            ImageFormat m_imgFormat;
            unsigned int m_memoryPitch;
            Backend m_backend;
            /**
             * @brief Indicates if this ImageContainer is bound to an object
             *
             */
            bool m_activeBound;
            /**
             * @brief total bytes stored
             *
             */
            int m_memsize;
            std::shared_ptr<std::mutex> m_mutexPtr;

        public:
            ImageContainer(Point<unsigned int> size, ImageFormat fmt);
            inline Point<unsigned int> getDims() { return m_dims; };
            inline unsigned long long int getFrameNumber() { return m_frameNumber; };
            inline BackendType getBackendType() { return m_backend.type; };
            inline BackendFlags getBackendFlags() { return m_backend.flags; };
            inline ImageFormat getImgFormat() { return m_imgFormat; };
            inline BackendFlags getFlags() { return m_backend.flags; };
            inline bool isBound() { return m_activeBound; };
            inline void setBound() { m_activeBound = true; };
            inline std::shared_ptr<std::mutex> getMutex() { return m_mutexPtr; };
            virtual const unsigned char *getConstPtr() const = 0;
            virtual unsigned char *getPtr() const = 0;
            virtual ContainerError updateMemory(unsigned long long int frame, const unsigned char *data, int size, Backend backend, int memPitch = 0) const = 0;
            virtual ContainerError updateMemory(std::shared_ptr<ImageContainer> img) const = 0;
        };

    };
};

#endif