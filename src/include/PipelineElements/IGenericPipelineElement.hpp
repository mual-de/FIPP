/**
 * @file IGenericPipelineElement.hpp
 * @author Alexander Mueller (dev@alexandermaxmueller.de)
 * @brief Generic Interface for all Pipeline Elements
 * @version 0.1
 * @date 2022-09-24
 * 
 * @copyright Copyright (c) 2022
 * 
 * Interface for all PipelineElements independend of their implementation.
 * 
 */
#ifndef __IGENERIC_PIPELINE_ELEMENT_HPP__
#define __IGENERIC_PIPELINE_ELEMENT_HPP__

#include <memory>
#include <string>
#include "../ImageContainer/ImageFormat.hpp"
#include "../ImageContainer/ImageContainer.hpp"
namespace FIPP
{

    namespace pipe
    {
        class IGenericPipelineElement
        {
        public:
            virtual bool startElement(img::ImageContainerConfig imgConfig, int predecessorId) = 0;
            virtual bool stopElement() = 0;
            virtual bool isRunning() = 0;
            virtual void connectPredecessor(int elemId) = 0;
            virtual void connectSuccessor(std::shared_ptr<IGenericPipelineElement> elem) = 0;
            virtual void addImageToInputPipe(std::shared_ptr<img::ImageContainer> img) = 0;
            virtual std::string getName() const = 0;
            virtual int getId() const = 0;

        protected:
            virtual void sendImageToSucessors(std::shared_ptr<img::ImageContainer> img) = 0;
            /**
             * @brief Overwrite by derived plugin to do calculation on next image
             * 
             * @param img 
             */
            virtual void doCalculation(std::shared_ptr<img::ImageContainer> img) = 0;

        };

    };
};

#endif