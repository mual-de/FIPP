#ifndef __IIMAGE_CONTAINER_HPP__
#define __IIMAGE_CONTAINER_HPP__
#include "ImageFormat.hpp"
#include "MetaDataSystem.hpp"
#include "../Point.hpp"

#include <mutex>
#include <memory>
#include <variant>
#include <map>

namespace FIPP
{
    namespace img
    {
        /**
         * @brief Errors raised by copy operations
         *
         */
        typedef enum e_containerError
        {
            /**
             * @brief Everything is okay, copy was a success
             *
             */
            OKAY,
            UNKNOWN_ERROR,
            /**
             * @brief Backend is not compatible to this ImageContainer implementation
             *
             */
            INVALID_BACKEND,
            /**
             * @brief memsize does not match, not possible to copy data
             *
             */
            INVALID_SIZE,
            /**
             * @brief Format does not match, not possible to copy data
             *
             */
            INVALID_FORMAT,
            /**
             * @brief Error with CUDA operations @see ImageContainerCUDA for further informations.
             * ImageContainerCUDA::getCudaError will give further informations regarding this error.
             *
             */
            CUDA_RUNTIME_ERROR
        } ContainerError;

        /**
         * @brief Interface for all ImageContainers
         */
        class IImageContainer
        {
        public:
            virtual Point<unsigned int> getDims() const = 0;
            /**
             * @brief get the memory size (without pitch!)
             *
             * @return size_t
             */
            virtual size_t getMemSize() const = 0;
            /**
             * @brief Get the actual frame number
             *
             * @return unsigned long long int
             */
            virtual unsigned long long int getFrameNumber() const = 0;
            /**
             * @brief Get the backend type of this image container (CPU, CUDA etc.)
             *
             * @return @see ImageFormat::BackendType
             */
            virtual BackendType getBackendType() const = 0;
            /**
             * @brief Get the Backend Flags object
             *
             * @return BackendFlags
             */
            virtual BackendFlags getBackendFlags() const = 0;
            /**
             * @brief Get the Img Format object
             *
             * @return ImageFormat
             */
            virtual ImageFormat getImgFormat() const = 0;
            /**
             * @brief Get the Flags object
             *
             * @return BackendFlags
             */
            virtual BackendFlags getFlags() const = 0;
            /**
             * @brief states out if this image is bound to the pipeline or is free to use by the image pool
             *
             * @return true imagecontainer is in use
             * @return false imagecontainer is free to use
             */
            virtual bool isBound() const = 0;
            /**
             * @brief Set the imagecontainer bound (ONLY USED BY @see ImagePool)
             *
             */
            virtual void setBound() = 0;
            /**
             * @brief Set the imagecontainer to unbound (Used by every Plugin/Sink that is an endpoint)
             *
             */
            virtual void setUnBound() = 0;
            /**
             * @brief Get a pointer to the whole metadata stored inside this Imagecontainer
             *
             * @return std::shared_ptr<MetaDataMapNode>
             */
            virtual std::shared_ptr<MetaDataMapNode> getMetaData() = 0;
            /**
             * @brief set updated flag true to force a download if request a cpu memory from a cuda/opencl img container
             *
             */
            virtual void setUpdated() = 0;

            /**
             * @brief Add a new metadata node to this imagecontainer meta data structure
             *
             * @param key to store the meta data and call informations
             * @param meta MetaDataNode containing the information
             */
            virtual void addMetaData(std::string key, std::shared_ptr<MetaDataNode> meta) = 0;
            /**
             * @brief Get the meta data which corresponds to the given key or the complete structure
             *
             * @param key
             * @return std::shared_ptr<MetaDataNode>
             */
            virtual std::shared_ptr<MetaDataNode> getMetaData(std::string key = "none") = 0;
            /**
             * @brief Get uuid of this imagecontainer
             *
             * @return const unsigned int
             */
            virtual const unsigned int getUUID() const = 0;
            /**
             * @brief Get a mutex for write operations.
             *
             * @return std::shared_ptr<std::mutex>
             */
            virtual std::shared_ptr<std::mutex> getMutex() = 0;
            /**
             * @brief Get ptr to const host memory, if not Unified/Zero Copy memory this will return nullptr!
             *
             * @return unsigned char*
             */
            virtual const unsigned char *getConstPtr() const = 0;
            /**
             * @brief Get ptr to host memory, if not Unified/Zero Copy memory this will return nullptr!
             *
             * @return unsigned char*
             */
            virtual unsigned char *getPtr() const = 0;
            /**
             * @brief Set the actual frame number
             *
             * @param frameNumber
             */
            virtual void setFrameNumber(unsigned long long int frameNumber) = 0;
            /**
             * @brief Update internal memory with content from data array
             *
             * If BackendType is UNIFIED_MEMORY or ZERO_COPY data must be the device pointer in ImageContainerCUDA.
             * In ImageContainerCPU this pointer needs to be a host pointer.
             * @param frame
             * @param data pointer to data (device_ptr for CUDA, host_ptr for CPU)
             * @param dims dimensions of the dataset
             * @param bytesPerPixel
             * @param backend
             * @param memPitch
             * @return ContainerError
             */
            virtual ContainerError updateMemory(unsigned long long int frame, const unsigned char *data, Point<unsigned int> dims, int bytesPerPixel, Backend backend, int memPitch) = 0;
            /**
             * @brief Update internal memory with content from other image container
             *
             * @param img
             * @return ContainerError
             */
            virtual ContainerError updateMemory(std::shared_ptr<IImageContainer> img) = 0;

        };
    }
}
#endif