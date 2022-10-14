/**
 * @file IGenericPlugin.hpp
 * @author Alexander Mueller (dev@alexandermaxmueller.de)
 * @brief Interface for a Generic Plugin as PipelineElement
 * @version 0.1
 * @date 2022-10-14
 * 
 * @copyright Copyright (c) 2022
 * 
 */
#ifndef __IGENERIC__PLUGIN_HPP__
#define __IGENERIC__PLUGIN_HPP__

#include <memory>
#include <string>
#include "IGenericSink.hpp"
#include "../ImageContainer/ImageFormat.hpp"
#include "../ImageContainer/ImageContainer.hpp"
namespace FIPP
{

    namespace pipe
    {

        class IGenericPlugin : public IGenericSink
        {
        public:
            /**
             * @brief tell this plugin which is/are the next plugins to send the result of this plugin to these plugins.
             * 
             * @param elem 
             */
            virtual void connectSuccessor(std::shared_ptr<IGenericSink> elem) = 0;
            /**
             * @brief Calls internal doCalculation function for a testrun without a running thread.
             * 
             * Simplifies testing with unit testing, put an image in and check if result is as expected.
             * @param img to run processing on
             * @return std::shared_ptr<img::ImageContainer> return value from doCalculation
             */
            virtual std::shared_ptr<img::ImageContainer> testInternalFunction(std::shared_ptr<img::ImageContainer> img) = 0;
        };

    };
};

#endif