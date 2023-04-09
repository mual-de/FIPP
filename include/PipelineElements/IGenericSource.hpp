/**
 * @file IGenericSource.hpp
 * @author Alexander Mueller (dev@alexandermaxmueller.de)
 * @brief A generic interface for all pipeline sources
 * @version 0.1
 * @date 2023-01-25
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef __IGENERIC__SOURCE_HPP__
#define __IGENERIC__SOURCE_HPP__

#include <memory>
#include <string>
#include "IGenericPipelineElement.hpp"

namespace FIPP
{

    namespace pipe
    {

        class IGenericSink;
        class IGenericSource : public IGenericPipelineElement
        {
        public:
            /**
             * @brief Connect a successor in pipeline order to this source.
             * @remark every plugin is handled as sink internally! (plugin extends sink)
             * @param elem ptr to sink/plugin
             */
            virtual void connectSuccessor(std::shared_ptr<IGenericSink> elem) = 0;
            /**
             * @brief Check if connection is possible and pipeline would start
             * 
             * @return true pipeline is interoperable and can start
             * @return false pipeline will fail
             */
            virtual bool interogateConnection() = 0;
        };

    };
};

#endif