#ifndef __IMAGE_CONTAINER_HPP__
#define __IMAGE_CONTAINER_HPP__

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
            
            std::shared_ptr<MetaDataMapNode> m_metaData;

        public:
            ImageContainer(Point<unsigned int> size, ImageFormat fmt, unsigned int uuid);
            inline Point<unsigned int> getDims() { return m_dims; };
            inline unsigned long long int getFrameNumber() { return m_frameNumber; };
            inline BackendType getBackendType() { return m_backend.type; };
            inline BackendFlags getBackendFlags() { return m_backend.flags; };
            inline ImageFormat getImgFormat() { return m_imgFormat; };
            inline BackendFlags getFlags() { return m_backend.flags; };
            inline bool isBound() const { return m_activeBound; };
            inline void setBound()
            {
                m_mutexPtr->lock();
                m_activeBound = true;
                m_mutexPtr->unlock();
            };
            inline void setUnBound()
            {
                m_mutexPtr->lock();
                m_activeBound = false;
                m_mutexPtr->unlock();
            };

            inline std::shared_ptr<MetaDataMapNode> getMetaData()
            {
                return this->m_metaData;
            }
            inline void addMetaData(std::string key, std::shared_ptr<MetaDataNode> meta)
            {
                if (this->m_metaData == nullptr)
                {
                    this->m_metaData = std::make_shared<MetaDataMapNode>();
                }
                this->m_metaData->addNode(key, meta);
            }

            inline std::shared_ptr<MetaDataNode> getMetaData(std::string key = "none")
            {
                if (key.compare("none") == 0)
                {
                    return m_metaData;
                }
                return this->m_metaData->getNode(key);
            }
            inline const unsigned int getUUID() const { return m_uuid; };
            inline std::shared_ptr<std::mutex> getMutex() { return m_mutexPtr; };
            virtual const unsigned char *getConstPtr() const = 0;
            virtual unsigned char *getPtr() const = 0;
            inline void setFrameNumber(unsigned long long int frameNumber){m_frameNumber = frameNumber;};
            virtual ContainerError updateMemory(unsigned long long int frame, const unsigned char *data, int size, Backend backend, int memPitch = 0) = 0;
            virtual ContainerError updateMemory(std::shared_ptr<ImageContainer> img) = 0;
        };

    };
};

#endif