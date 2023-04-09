#ifndef __IIMAGE_CONTAINER_CUDA_HPP__
#define __IIMAGE_CONTAINER_CUDA_HPP__
#include "IImageContainer.hpp"

#include "cuda_runtime.h"



namespace FIPP
{

    namespace img
    {
        class IImageContainerCUDA : public virtual IImageContainer
        {
        public:
            /**
             * @brief Get const pointer to device memory
             *
             * @return unsigned char*
             */
            virtual const unsigned char *getConstDevPtr() const = 0;
            /**
             * @brief Get pointer to device memory
             *
             * Use lock before using this function to write into this image container!
             *
             * @return unsigned char*
             */
            virtual unsigned char *getDevPtr() const = 0;
            virtual bool allowsCPUAccess() = 0;
            /**
             * @brief if memory is allocated for device it is possibly aligned with a pitch for better memory access.
             *
             * @return unsigned int
             */
            virtual unsigned int getMemPitch() const = 0;
            /**
             * @brief Get the last raised cuda error
             *
             * @return cudaError_t
             */
            virtual cudaError_t getLastCudaErr() const = 0;
            /**
             * @brief Get the last raised cude error code!
             *
             * @return std::string
             */
            virtual std::string getLastCudaErrCode() = 0;
        };
    }
}

#endif