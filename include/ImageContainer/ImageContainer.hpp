#ifndef __IMAGE_CONTAINER_HPP__
#define __IMAGE_CONTAINER_HPP__
/**
 * @file ImageContainer.hpp
 * @author Alexander Mueller (dev@alexandermaxmueller.de)
 * @brief A abstract ImageContainer class to be inheritated by ImageContainerCPU and ImageContainerCUDA to store informations.
 * @version 0.1
 * @date 2023-01-21
 *
 * @copyright Copyright (c) 2023
 *
 */
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
         * @brief
         *
         */
        class ImageContainer
        {
        protected:
            Point<unsigned int> m_dims;
            /**
             * @brief Actual frame number if img in this container.
             *
             */
            unsigned long long int m_frameNumber;
            /**
             * @brief ImageFormat of this imageContainer, can't be changed during runtime.
             *
             */
            ImageFormat m_imgFormat;
            /**
             * @brief memory pitch is provided by cuda and other libs to store data more efficient.
             *
             */
            unsigned int m_memoryPitch;
            /**
             * @brief Backendtype of this container (e.g. CUDA or CPU)
             *
             */
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
            /**
             * @brief Mutex if internal data will be changed.
             *
             */
            std::shared_ptr<std::mutex> m_mutexPtr;
            /**
             * @brief uuid specific to every image pool
             *
             */
            const unsigned int m_uuid;
            /**
             * @brief MetData to store further informations inside the image container.
             * 
             */
            std::shared_ptr<MetaDataMapNode> m_metaData;

            bool m_updated = false;

        public:
            ImageContainer(Point<unsigned int> size, ImageFormat fmt, unsigned int uuid);
            /**
             * @brief Get the dimension of the stored image.
             *
             * @return Point<unsigned int>
             */
            inline Point<unsigned int> getDims() const { return m_dims; };
            /**
             * @brief get the memory size (without pitch!)
             * 
             * @return size_t 
             */
            inline size_t getMemSize() const {return static_cast<size_t>(m_memsize);}
            /**
             * @brief Get the actual frame number
             *
             * @return unsigned long long int
             */
            inline unsigned long long int getFrameNumber() { return m_frameNumber; };
            /**
             * @brief Get the backend type of this image container (CPU, CUDA etc.)
             *
             * @return @see ImageFormat::BackendType
             */
            inline BackendType getBackendType() { return m_backend.type; };
            /**
             * @brief Get the Backend Flags object
             *
             * @return BackendFlags
             */
            inline BackendFlags getBackendFlags() { return m_backend.flags; };
            /**
             * @brief Get the Img Format object
             *
             * @return ImageFormat
             */
            inline ImageFormat getImgFormat() { return m_imgFormat; };
            /**
             * @brief Get the Flags object
             *
             * @return BackendFlags
             */
            inline BackendFlags getFlags() { return m_backend.flags; };
            /**
             * @brief states out if this image is bound to the pipeline or is free to use by the image pool
             *
             * @return true imagecontainer is in use
             * @return false imagecontainer is free to use
             */
            inline bool isBound() const { return m_activeBound; };
            /**
             * @brief Set the imagecontainer bound (ONLY USED BY @see ImagePool)
             *
             */
            inline void setBound()
            {
                m_mutexPtr->lock();
                m_activeBound = true;
                m_mutexPtr->unlock();
            };
            /**
             * @brief Set the imagecontainer to unbound (Used by every Plugin/Sink that is an endpoint)
             *
             */
            inline void setUnBound()
            {
                m_mutexPtr->lock();
                m_activeBound = false;
                m_mutexPtr->unlock();
            };
            /**
             * @brief Get a pointer to the whole metadata stored inside this Imagecontainer
             *
             * @return std::shared_ptr<MetaDataMapNode>
             */
            inline std::shared_ptr<MetaDataMapNode> getMetaData()
            {
                return this->m_metaData;
            }
            /**
             * @brief set updated flag true to force a download if request a cpu memory from a cuda/opencl img container
             *
             */
            inline void setUpdated()
            {
                this->m_updated = true;
            }

            /**
             * @brief Add a new metadata node to this imagecontainer meta data structure
             *
             * @param key to store the meta data and call informations
             * @param meta MetaDataNode containing the information
             */
            inline void addMetaData(std::string key, std::shared_ptr<MetaDataNode> meta)
            {
                if (this->m_metaData == nullptr)
                {
                    this->m_metaData = std::make_shared<MetaDataMapNode>();
                }
                this->m_metaData->addNode(key, meta);
            }
            /**
             * @brief Get the meta data which corresponds to the given key or the complete structure
             *
             * @param key
             * @return std::shared_ptr<MetaDataNode>
             */
            inline std::shared_ptr<MetaDataNode> getMetaData(std::string key = "none")
            {
                if (key.compare("none") == 0)
                {
                    return m_metaData;
                }
                return this->m_metaData->getNode(key);
            }
            /**
             * @brief Get uuid of this imagecontainer
             *
             * @return const unsigned int
             */
            inline const unsigned int getUUID() const { return m_uuid; };
            /**
             * @brief Get a mutex for write operations.
             * 
             * @return std::shared_ptr<std::mutex> 
             */
            inline std::shared_ptr<std::mutex> getMutex() { return m_mutexPtr; };
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
            inline void setFrameNumber(unsigned long long int frameNumber) { m_frameNumber = frameNumber; };
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
            virtual ContainerError updateMemory(std::shared_ptr<ImageContainer> img) = 0;
        };

    };
};

#endif