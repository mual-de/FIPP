#include "PipelineElements/InputFilter.hpp"

using namespace FIPP::pipe;
InputFilter::InputFilter(Point<unsigned int> maxDims)
{
    this->m_maxDims = maxDims;
}

InputFilter::InputFilter()
{
    this->m_maxDims = Point<unsigned int>(20000,20000);
}

bool InputFilter::checkIfCompatible(img::ImageContainerConfig config)
{
    if (config.dimensions > this->m_maxDims)
    {
        return false;
    }
    if (this->m_minDimsSet)
    {
        if (config.dimensions < this->m_minDims)
        {
            return false;
        }
    }
    bool supported = false;
    for (const auto e : this->m_supportedBackendTypes)
    {
        if (e == config.backend.type)
        {
            supported = true;
            break;
        }
    }
    if (!supported)
    {
        return false;
    }
    supported = false;
    for (const auto e : this->m_supportedImageTypes)
    {
        if (e == config.imgFormat.imgType)
        {
            supported = true;
            break;
        }
    }
    if (!supported)
    {
        return false;
    }
    return true;
}