/**
 * @file ImageFormat.hpp
 * @author Alexander Mueller (dev@alexandermaxmueller.de)
 * @brief All definitions for image formats and backend types
 * @version 0.1
 * @date 2022-11-19
 *
 * @copyright Copyright (c) 2022
 *
 * Header containing all relevant image formats and backend types as enums.
 * Provides functions for converting from std::string to these enums.
 *
 */

#ifndef __IMAGE_FORMAT_HPP__
#define __IMAGE_FORMAT_HPP__
#include "../Point.hpp"
#include <map>
#include <stdexcept>
namespace YAML{
    class Node;
};

namespace FIPP
{
    namespace img
    {
        /**
         * @brief All image types (RGB, RGBA, GRAY usw.)
         */
        typedef enum e_ImageType
        {
            RGB,
            RGBA,
            GRAY
        } ImageType;

        /**
         * @brief conversion map from std::string to ImageType
         */
        static std::map<std::string, ImageType> const stringToImgType = {{"RGB", ImageType::RGB}, {"RGBA", ImageType::RGBA}, {"GRAY", ImageType::GRAY}};

        /**
         * @brief getImageType for a given string
         * @param string with image type name
         * @return ImageType enum value
         * @throws invalid_argument if given string is not known.
         */
        static ImageType getImageType(std::string imgType)
        {
            if (stringToImgType.find(imgType) == stringToImgType.end())
            {
                throw std::invalid_argument("Image Type doesn't exist!");
            }
            return stringToImgType.at(imgType);
        }

        /**
         * @brief All supported backend types.
         */
        typedef enum e_BackendType
        {
            CPU,
            CUDA,
            OPENCL
        } BackendType;

        /**
         * @brief conversion map from std::string to BackendType
         */
        static std::map<std::string, BackendType> const stringToBackendType = {{"CPU", BackendType::CPU}, {"CUDA", BackendType::CUDA}, {"OPENCL", BackendType::OPENCL}};

        /**
         * @brief getBackendType for a given string
         * @param string with backend name
         * @return getBackendType enum value
         * @throws invalid_argument if given string is not known.
         */
        static BackendType getBackendType(std::string imgType)
        {
            if (stringToBackendType.find(imgType) == stringToBackendType.end())
            {
                throw std::invalid_argument("Image Type doesn't exist!");
            }
            return stringToBackendType.at(imgType);
        }

        /**
         * @brief Flags for Backend Systems
         * Necessary for example for CUDA (Memory should be accessable via GPU, ZERO COPY OR UNIFIED MEM?)
         */
        typedef enum e_BackendFlags
        {
            CPU_ONLY = 2,
            GPU_ONLY = 4,
            ZERO_COPY = 8,
            UNIFIED_MEMORY = 16
        } BackendFlags;

        /**
         * @brief Complete definition of an ImageFormat
         *
         * Containing the type (RGB, RGBA, GRAY), the number of bits per Pixel/ColorLayer (e.g. 12 Bit) and the bytesPerPixel(e.g. 3 for an RGB 8-bit image).
         */
        typedef struct e_ImageFormat
        {
            ImageType imgType;
            unsigned char bitDepthPerPixel;
            unsigned char bytesPerPixel;
            inline bool operator==(e_ImageFormat a) { return (bitDepthPerPixel == a.bitDepthPerPixel) && (a.bytesPerPixel == bytesPerPixel) && (a.imgType == imgType); };
            inline bool operator!=(e_ImageFormat a) { return !((bitDepthPerPixel == a.bitDepthPerPixel) && (a.bytesPerPixel == bytesPerPixel) && (a.imgType == imgType)); };
        } ImageFormat;

        /**
         * @brief Backend struct combining @see BackendType and @see BackendFlags
         */
        typedef struct e_Backend
        {
            BackendType type;
            BackendFlags flags;
        } Backend;

        typedef struct e_ImageContainerConfig
        {
            ImageFormat imgFormat;
            Backend backend;
            FIPP::Point<unsigned int> dimensions;
        } ImageContainerConfig;

        ImageContainerConfig getContainerConfigFromYaml(YAML::Node containerNode);

    };
}
#endif