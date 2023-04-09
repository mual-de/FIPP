/**
 * @file ImageContainerCUDA.hpp
 * @author Alexander Mueller (dev@alexandermaxmueller.de)
 * @brief Implementation of an ImageContainer with CUDA Backend for processing directly on a gpu.
 * @version 0.1
 * @date 2023-03-09
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef __IMAGE_CONTAINER_CUDA_HPP__
#define __IMAGE_CONTAINER_CUDA_HPP__

#include "ImageContainer.hpp"
#include "ImageContainer/IImageContainerCUDA.hpp"

namespace FIPP
{
    namespace img
    {
        class ImageContainerCUDA : public ImageContainer, public IImageContainerCUDA
        {

        public:
        /**
         * @brief Construct a new Image Container C U D A object
         * 
         * @param size dimension of this image container
         * @param format
         * @param flags 
         * @param uuid 
         */
            ImageContainerCUDA(Point<unsigned int> size, ImageFormat format, BackendFlags flags, unsigned int uuid);
            /**
             * @brief Destroy the Image Container C U D A object
             * 
             */
            ~ImageContainerCUDA();
            /**
             * @brief Get const ptr to host memory, if not Unified/Zero Copy memory this will return nullptr!
             * 
             * @return const unsigned char* 
             */
            const unsigned char *getConstPtr() const override;
            /**
             * @brief Get ptr to host memory, if not Unified/Zero Copy memory this will return nullptr!
             * 
             * @return unsigned char* 
             */
            unsigned char *getPtr() const override;
            /**
             * @brief Get const pointer to device memory
             * 
             * @return unsigned char* 
             */
            const unsigned char *getConstDevPtr() const;
            /**
             * @brief Get pointer to device memory
             * 
             * Use lock before using this function to write into this image container!
             * 
             * @return unsigned char* 
             */
            unsigned char *getDevPtr() const;
            ContainerError updateMemory(unsigned long long int frame, const unsigned char *data, Point<unsigned int> dims, int bytesPerPixel, Backend backend, int memPitch) override;
            /**
             * @brief Copy the content of an ImageContainer into this ImageContainer.
             * 
             * Every ImageContainer can be an Input.
             * @throws runtime_error 
             * 
             * @param img 
             * @return ContainerError 
             */
            ContainerError updateMemory(std::shared_ptr<IImageContainer> img) override;
            /**
             * @brief Check if CPU access is possible without checking for Unified Memory/Zero Copy memory!
             * 
             * @return true CPU access is possible!
             * @return false CPU access is not possible!
             */
            bool allowsCPUAccess() {return this->m_allowsCPUAccess;};
            /**
             * @brief if memory is allocated for device it is possibly aligned with a pitch for better memory access.
             * 
             * @return unsigned int 
             */
            unsigned int getMemPitch() const override {return this->m_memoryPitch;};
            /**
             * @brief Get the last raised cuda error
             * 
             * @return cudaError_t 
             */
            cudaError_t getLastCudaErr() const override;
            /**
             * @brief Get the last raised cude error code!
             * 
             * @return std::string 
             */
            std::string getLastCudaErrCode();

        private:
            /**
             * @brief Flag if cpu access is possible (via getConstPtr/getPtr)
             * Only possible with unified and zero copy memory!
             * 
             */
            bool m_allowsCPUAccess;
            /**
             * @brief Pointer to device memory
             * 
             */
            unsigned char* m_cpuBuffer;
            /**
             * @brief Pointer to host memory
             * 
             */
            unsigned char* m_gpuBuffer;
            /**
             * @brief Last raised cuda error for request
             * 
             */
            cudaError_t m_lastCudaErr;

            /**
             * @brief Allocate Memory for Device only
             * 
             */
            void allocCudaMem();
            /**
             * @brief Allocate Memory for Unified Memory
             * 
             */
            void allocCudaUnMem();
            /**
             * @brief Allocate Memory for Pinned/Zero Copy Memory
             * 
             */
            void allocCudaZCMem();
            /**
             * @brief Deallocate device memory
             * 
             */
            void deAllocCudaMem();
            /**
             * @brief Deallocate unified memory
             * 
             */
            void deAllocCudaUnMem();
            /**
             * @brief Deallocate pinned/zero copy memory
             * 
             */
            void deAllocCudaZCMem();

            /**
             * @brief Copy from cpu allocated memory to gpu memory
             * @param data 
             * @return cudaError code @see cudaError_t
             */
            cudaError_t copyFromCPU(const unsigned char *data);
            /**
             * @brief Copy from device to device memory
             * @throw runtime exception if cuda error occurs
             * @param img 
             * @return cudaError code @see cudaError_t
             */
            cudaError_t copyFromCUDA(std::shared_ptr<IImageContainerCUDA> img);
            /**
             * @brief Copy from device to device memory
             * @throw runtime exception if cuda error occurs
             * 
             * @param data 
             * @param flags 
             * @param memPitch 
             * @return cudaError code @see cudaError_t
             */
            cudaError_t copyFromCUDA(const unsigned char *data, BackendFlags flags, int memPitch);


        };
    }
}
#endif