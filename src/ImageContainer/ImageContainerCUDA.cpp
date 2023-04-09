#include "ImageContainerCUDA.hpp"
#include <cstring>

using namespace FIPP;
using namespace FIPP::img;
using namespace std;

ImageContainerCUDA::ImageContainerCUDA(Point<unsigned int> size, ImageFormat format, BackendFlags flags, unsigned int uuid): ImageContainer(size, format, uuid)
{
    this->m_backend.type = BackendType::CUDA;
    this->m_backend.flags = flags;
    this->m_lastCudaErr = cudaSuccess;

    switch (this->m_backend.flags)
    {
    case BackendFlags::GPU_ONLY:
        allocCudaMem();
        this->m_allowsCPUAccess = false;
        break;
    case BackendFlags::UNIFIED_MEMORY:
        allocCudaUnMem();
        this->m_allowsCPUAccess = true;
        break;
    default:
        allocCudaZCMem();
        this->m_allowsCPUAccess = true;
        break;
    }
}
ImageContainerCUDA::~ImageContainerCUDA()
{
    switch (this->m_backend.flags)
    {
    case BackendFlags::GPU_ONLY:
        deAllocCudaMem();
        break;
    case BackendFlags::UNIFIED_MEMORY:
        deAllocCudaUnMem();
        break;
    default:
        deAllocCudaZCMem();
        break;
    }
}

void ImageContainerCUDA::allocCudaMem()
{
    if (this->m_imgFormat.usePitch)
    {
        this->m_lastCudaErr = cudaMallocPitch((void **)&m_gpuBuffer, (size_t *)&m_memoryPitch, static_cast<size_t>(this->m_dims.getXUInt() * this->m_imgFormat.bytesPerPixel), static_cast<size_t>(this->m_dims.getYUInt()));
        if (this->m_lastCudaErr != cudaSuccess)
        {
            throw runtime_error("Error during memory allocation: " + string(cudaGetErrorString(this->m_lastCudaErr)));
        }
    }
    else
    {
        this->m_memoryPitch = 0;
        this->m_lastCudaErr = cudaMalloc((void **)&this->m_gpuBuffer, this->m_memsize);
        if (this->m_lastCudaErr != cudaSuccess)
        {
            throw runtime_error("Error during memory allocation: " + string(cudaGetErrorString(this->m_lastCudaErr)));
        }
    }
    this->m_cpuBuffer = nullptr;
}

void ImageContainerCUDA::allocCudaUnMem()
{
    this->m_memoryPitch = 0;
    this->m_lastCudaErr = cudaMallocManaged(&this->m_gpuBuffer, static_cast<size_t>(this->m_memsize));
    if (this->m_lastCudaErr != cudaSuccess)
    {
        throw runtime_error("Error during memory allocation: " + string(cudaGetErrorString(this->m_lastCudaErr)));
    }
    this->m_cpuBuffer = this->m_gpuBuffer;
}

void ImageContainerCUDA::allocCudaZCMem()
{
    cudaDeviceProp prop;
    this->m_lastCudaErr = cudaGetDeviceProperties(&prop, 0);
    if (!prop.canMapHostMemory)
    {
        throw runtime_error("Not possible to alloc zero copy memory on this gpu!");
    }
    this->m_lastCudaErr = cudaMallocHost((void **)&this->m_cpuBuffer, this->m_memsize);
    if (this->m_lastCudaErr != cudaSuccess)
    {
        throw runtime_error("Error during memory allocation: " + string(cudaGetErrorString(this->m_lastCudaErr)));
    }
    this->m_lastCudaErr = cudaHostGetDevicePointer((void **)&this->m_gpuBuffer, (void *)this->m_cpuBuffer, 0);
    if (this->m_lastCudaErr != cudaSuccess)
    {
        throw runtime_error("Can't fetch device ptr: " + string(cudaGetErrorString(this->m_lastCudaErr)));
    }
}

cudaError_t ImageContainerCUDA::getLastCudaErr() const
{
    return this->m_lastCudaErr;
}

std::string ImageContainerCUDA::getLastCudaErrCode()
{
    return string(cudaGetErrorString(this->m_lastCudaErr));
}

void ImageContainerCUDA::deAllocCudaMem()
{
    this->m_lastCudaErr = cudaFree(this->m_gpuBuffer);
}

void ImageContainerCUDA::deAllocCudaUnMem()
{
    this->m_lastCudaErr = cudaFree(this->m_gpuBuffer);
}

void ImageContainerCUDA::deAllocCudaZCMem()
{
    this->m_lastCudaErr = cudaFreeHost(this->m_cpuBuffer);
}

const unsigned char *ImageContainerCUDA::getConstPtr() const
{
    return this->m_cpuBuffer;
}

unsigned char *ImageContainerCUDA::getPtr() const
{
    return this->m_cpuBuffer;
}

const unsigned char *ImageContainerCUDA::getConstDevPtr() const
{
    return this->m_gpuBuffer;
}

unsigned char *ImageContainerCUDA::getDevPtr() const
{
    return this->m_gpuBuffer;
}

ContainerError ImageContainerCUDA::updateMemory(unsigned long long int frame, const unsigned char *data, Point<unsigned int> dims, int bytesPerPixel, Backend backend, int memPitch = 0)
{
    if (dims.getArea() * bytesPerPixel != this->m_memsize)
    {
        return ContainerError::INVALID_SIZE;
    }
    this->m_frameNumber = frame;
    switch (backend.type)
    {
    case BackendType::OPENCL:
        break;
    case BackendType::CUDA:
        this->m_lastCudaErr = this->copyFromCUDA(data, backend.flags, memPitch);
        break;
    default:
        this->m_lastCudaErr = this->copyFromCPU(data);
        break;
    }
    if (this->m_lastCudaErr != cudaSuccess)
    {
        return ContainerError::CUDA_RUNTIME_ERROR;
    }
    return ContainerError::OKAY;
}

cudaError_t ImageContainerCUDA::copyFromCPU(const unsigned char *data)
{
    cudaError_t err = cudaSuccess;
    switch (this->m_backend.flags)
    {
    case BackendFlags::GPU_ONLY:
        if (this->m_memoryPitch == 0)
        {
            err = cudaMemcpy(this->m_gpuBuffer, data, this->m_memsize, cudaMemcpyHostToDevice);
        }
        else
        {
            err = cudaMemcpy2D(this->m_gpuBuffer, this->m_memoryPitch, data, 0, this->m_dims.getX(), this->m_dims.getY(), cudaMemcpyHostToDevice);
        }
        break;
    case BackendFlags::UNIFIED_MEMORY:
        err = cudaMemcpy(this->m_gpuBuffer, data, this->m_memsize, cudaMemcpyHostToDevice);
        break;
    case BackendFlags::ZERO_COPY:
        err = cudaMemcpy(this->m_gpuBuffer, data, this->m_memsize, cudaMemcpyHostToDevice);
        break;
    default:
        memcpy(this->m_cpuBuffer, data, this->m_memsize);
    }
    return err;
};

cudaError_t ImageContainerCUDA::copyFromCUDA(const unsigned char *data, BackendFlags flags, int memPitch = 0)
{

    cudaError_t err = cudaMemcpy2D(this->m_gpuBuffer, this->m_memoryPitch, data, memPitch, this->m_dims.getX(), this->m_dims.getY(), cudaMemcpyDeviceToDevice);
    return err;
}

cudaError_t ImageContainerCUDA::copyFromCUDA(std::shared_ptr<IImageContainerCUDA> imgCuda)
{
    return this->copyFromCUDA(imgCuda->getConstDevPtr(), imgCuda->getBackendFlags(), imgCuda->getMemPitch());
}

ContainerError ImageContainerCUDA::updateMemory(shared_ptr<IImageContainer> img)
{
    if (img->getMemSize() != this->m_memsize)
    {
        return ContainerError::INVALID_SIZE;
    }
    this->m_frameNumber = img->getFrameNumber();
    switch (img->getBackendType())
    {
    case BackendType::OPENCL:
        break;
    case BackendType::CUDA:
        this->m_lastCudaErr = this->copyFromCUDA(dynamic_pointer_cast<IImageContainerCUDA>(img));
        break;
    default:
        this->m_lastCudaErr = this->copyFromCPU(img->getConstPtr());
        break;
    }
    if (this->m_lastCudaErr != cudaSuccess)
    {
        return ContainerError::CUDA_RUNTIME_ERROR;
    }
    return ContainerError::OKAY;
}
