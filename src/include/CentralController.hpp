#ifndef __CENTRAL_CONTROLLER_HPP__
#define __CENTRAL_CONTROLLER_HPP__

#include <memory>
#include <map>

namespace FIPP{
    namespace pipe{
        class IGenericPipelineElement;
    }
    namespace img{
        class ImageContainer;
    }
    namespace logging
    {
        class ILogger;
    } // namespace name
    
    class CentralController{

        public:
            CentralController();
            void start();
            void stop();
            std::shared_ptr<img::ImageContainer> getImageContainerForPipe(int pipeId);
            void sendImageToPipe(std::shared_ptr<img::ImageContainer> img, int pipeId);
        private:
            std::map<int, std::shared_ptr<pipe::IGenericPipelineElement>> m_pipes;
            std::shared_ptr<logging::ILogger> m_log;
            void createLogger();
            void createPipelineElements();
    };
}

#endif