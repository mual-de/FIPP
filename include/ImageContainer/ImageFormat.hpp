#ifndef __IMAGE_FORMAT_HPP__
#define __IMAGE_FORMAT_HPP__

namespace FIPP
{
    namespace img
    {
        typedef enum e_ImageType
        {
            RGB,
            RGBA,
            GRAY
        } ImageType;

        typedef enum e_BackendType
        {
            CPU,
            CUDA,
            OPENCL
        } BackendType;

        typedef enum e_BackendFlags
        {
            CPU_ONLY = 2,
            GPU_ONLY = 4,
            ZERO_COPY = 8,
            UNIFIED_MEMORY = 16
        } BackendFlags;

        typedef struct e_ImageFormat
        {
            ImageType imgType;
            unsigned char bitDepthPerPixel;
            unsigned char bytesPerPixel;
            inline bool operator==(e_ImageFormat a) { return (bitDepthPerPixel == a.bitDepthPerPixel) && (a.bytesPerPixel == bytesPerPixel) && (a.imgType == imgType); };
            inline bool operator!=(e_ImageFormat a) { return !((bitDepthPerPixel == a.bitDepthPerPixel) && (a.bytesPerPixel == bytesPerPixel) && (a.imgType == imgType));};
        } ImageFormat;

        typedef struct e_Backend
        {
            BackendType type;
            BackendFlags flags;
        } Backend;

        typedef struct e_ImageContainerConfig
        {
            ImageFormat imgFormat;
            Backend backend;
        } ImageContainerConfig;
    };
}
#endif