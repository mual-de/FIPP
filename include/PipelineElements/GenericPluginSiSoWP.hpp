/**
 * @file GenericPluginSiSoWP.hpp
 * @author Alexander Mueller (dev@alexandermaxmueller.de)
 * @brief Generic Plugin Single Input Single Outpur with ImagePool
 * @version 0.1
 * @date 2022-10-15
 * 
 * @copyright Copyright (c) 2022
 * 
 * Superclass for Generic Plugins with single input and single output
 * which will do image conversion where the output will not fit into
 * a input image container (e.g. other format, other resolution).
 */
#ifndef __GENERIC_PLUGIN_SISO_WP_HPP__
#define __GENERIC_PLUGIN_SISO_WP_HPP__
#include "GenericPlugin.hpp"
#include "../Logging/ILogging.hpp"
#include "../ImageContainer/ImagePool.hpp"
#include <memory>
#include <string>
#include <mutex>
#include <queue>
#include "InputFilter.hpp"

namespace FIPP
{
    namespace pipe
    {
        class GenericPluginSiSoWP : public GenericPlugin
        {
        public:
            GenericPluginSiSoWP(std::string elemName, int elemId, std::shared_ptr<FIPP::logging::ILogger> log);
            virtual ~GenericPluginSiSoWP(){};
            bool startElement(int predecessorId);
            bool stopElement();
            void connectPredecessor(int elemId);
            void connectSuccessor(std::shared_ptr<IGenericSink> elem);
            bool interogateConnection(img::ImageContainerConfig imgConfig, int predecessorId);

        protected:
            /**
             * @brief Ptr to the next element in pipe
             * 
             */
            std::shared_ptr<IGenericSink> m_successor;
            /**
             * @brief ptr to image pool created at startElement, will be released at stopElement
             * 
             */
            std::unique_ptr<img::ImagePool> m_pool;
            /**
             * @brief Pool Size given by configuration
             * 
             */
            int m_poolSize;

            int m_predecessorId;
            InputFilter m_filter;
            /**
             * @brief Configuration for outgoing images
             * 
             */
            img::ImageContainerConfig m_outputConfiguration;
            void sendImageToSucessors(std::shared_ptr<img::ImageContainer> img);
            /**
             * @brief Overwrite by derived plugin to do calculation on next image
             *
             * @param img
             */
            virtual std::shared_ptr<img::ImageContainer> doCalculation(std::shared_ptr<img::ImageContainer> img) = 0;

        };

    };
};

#endif