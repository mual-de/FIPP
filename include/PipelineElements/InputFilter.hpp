#ifndef __INPUT_FILTER_HPP__
#define __INPUT_FILTER_HPP__

#include "../Point.hpp"
#include "../ImageContainer/ImageFormat.hpp"
#include <vector>
#include <memory>

namespace FIPP{
    namespace pipe{
        class InputFilter{
            public:
                InputFilter(Point<unsigned int> maxDims);
                InputFilter();
                /**
                 * @brief Add a supported backend type to this input filter
                 * 
                 * @param beType 
                 */
                inline void addSupportedBackendType(img::BackendType beType){this->m_supportedBackendTypes.push_back(beType);};
                inline void addSupportedImageType(img::ImageType imgType){this->m_supportedImageTypes.push_back(imgType);};
                inline void addMinimumDims(Point<unsigned int> minDims){ m_minDimsSet = true; m_minDims = minDims;};
                inline void addMaxDims(Point<unsigned int> maxDims){m_maxDims = maxDims;};
                bool checkIfCompatible(img::ImageContainerConfig config);
            private:
                std::vector<img::BackendType> m_supportedBackendTypes;
                std::vector<img::ImageType> m_supportedImageTypes;
                Point<unsigned int> m_maxDims;
                bool m_minDimsSet;
                Point<unsigned int> m_minDims;
        };
    }
}

#endif