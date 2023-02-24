#ifndef __PIPELINE_CONTROLLER_HPP__
#define __PIPELINE_CONTROLLER_HPP__

#include <memory>
#include <map>

namespace YAML
{
    class Node;
};

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
    
    class PipelineController{

        public:
            PipelineController(YAML::Node config);
            ~PipelineController();
            void start();
            void stop();
            std::shared_ptr<img::ImageContainer> getImageContainerForPipe(int pipeId);
            void sendImageToPipe(std::shared_ptr<img::ImageContainer> img, int pipeId);
        private:
            std::map<int, std::shared_ptr<pipe::IGenericPipelineElement>> m_pipes;
            /**
             * @brief Standard system logger from central controller
             *
             */
            std::shared_ptr<logging::ILogger> m_log;
            /**
             * @brief set of uuids already in use by pipeline.
             *
             */
            std::set<int> m_idsInUsage;
            /**
             * @brief map of available plugins and their correspondent constructors.
            */
            std::map<std::string, void*> m_plugins;
            std::shared_ptr<logging::ILogger> m_log;
            void createLogger();
            void createPipelineElements();
    };
}

#endif