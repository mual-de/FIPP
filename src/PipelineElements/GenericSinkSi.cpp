
#include "PipelineElements/GenericSinkSi.hpp"
#include "ImageContainer/ImageFormat.hpp"
#include "Logging/ILogging.hpp"
using namespace FIPP::pipe;
using namespace FIPP::logging;

#define LOG(lvl, msg) this->m_log->log(lvl, msg, "GenericSinkSi", __func__, __LINE__, this->m_frameNumber);

bool GenericSinkSi::interogateConnection(img::ImageContainerConfig imgConfig, int predecessorId)
{
    if(this->m_predeccessor != predecessorId){
        LOG(LogLevel::ERROR, "Not requested by valid predecessor!");
        return false;
    }
    if (this->m_filterActivated)
    {
        if (this->m_filter.checkIfCompatible(imgConfig))
        {
            LOG(LogLevel::ERROR, "Image Formats are not compatibel!");
            return true;
        }else{
            return false;
        }
    }else{
        return true;
    }
}