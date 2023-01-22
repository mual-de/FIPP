/**
 * @file MetaDataSystem.hpp
 * @author Alexander Mueller (dev@alexandermaxmueller.de)
 * @brief Implementation for a MetaDataSystem to store meta data next to image data in ImageContainer
 * @version 0.1
 * @date 2023-01-22
 * 
 * @copyright Copyright (c) 2023
 * 
 */
#ifndef __META_DATA_SYSTEM_HPP__
#define __META_DATA_SYSTEM_HPP__
#include <map>
#include <memory>
#include <vector>
#include "../Point.hpp"

namespace FIPP
{
    namespace img
    {

/**
 * @brief Different kind of MetaData Nodes
 * 
 */
        typedef enum e_METATYPES
        {
            INT,
            UNSIGNED_INT,
            FLOAT,
            CHAR,
            BOOL,
            POINT_INT,
            POINT_FLOAT,
            POINT_UNSIGNED_INT,
            STRING,
            VECTOR_INT,
            VECTOR_UNSIGNED_INT,
            VECTOR_FLOAT,
            VECTOR_CHAR,
            VECTOR_BOOL,
            VECTOR_POINT_INT,
            VECTOR_POINT_FLOAT,
            VECTOR_POINT_UNSIGNED_INT,
            VECTOR_STRING,
            MAP,
            NONE
        } MetaTypes;
        /**
             * @brief Get Type of this node provided by @see MetaTypes
             *
             */
        class MetaDataNode
        {
        public:
            /**
             * @brief Get Type of this node provided by @see MetaTypes
             * @return node type as MetaTypes
             */
            virtual const MetaTypes getType() const = 0;
        };

        /**
         * @brief Generic Value Node to store information about the Node type next to values.
         * 
         */
        class MetaDataValueNodeGeneric: public MetaDataNode{
            public:
                MetaDataValueNodeGeneric(MetaTypes type) : m_type(type){};
                /**
                 * @brief Get value of this MetaDataValueNode
                 * 
                 * @return const MetaTypes 
                 */
                const MetaTypes getType() const {return this->m_type;};
            protected:
                const MetaTypes m_type;
        };

        
        /**
         * @brief Implementation of MetaDataValueNodeGeneric for simple values (int, float etc.)
         * 
         * @tparam T type of node
         */
        template <typename T>
        class MetaDataValueNode : public MetaDataValueNodeGeneric{
            public:
                MetaDataValueNode(T value);
                /**
                 * @brief Get the value of this node
                 * 
                 * @return const T 
                 */
                const T getValue() const{return this->m_value;};
            private:
                /**
                 * @brief value to be stored.
                 * 
                 */
                const T m_value;
        };
        /**
         * @brief Implementation of MetaDataValueNodeGeneric for simple vectors (int, float etc.)
         * 
         * @tparam T vector type (e.g. vector<int>)
         */
        template <typename T>
        class MetaDataVectorNode : public MetaDataValueNodeGeneric{
            public:
                MetaDataVectorNode(std::vector<T> vector);
                /**
                 * @brief Get the vector by value
                 * 
                 * @return const std::vector<T> 
                 */
                const std::vector<T> getValue() const {return this->m_values;};
                /**
                 * @brief Get the vector by pointer (faster access)
                 * 
                 * @return const std::vector<T>* 
                 */
                const std::vector<T>* getValuePtr() {return &this->m_values;};
            private:
                /**
                 * @brief internal vector to store information
                 * 
                 */
                const std::vector<T> m_values;

        };

        /**
         * @brief A node of MetaDataNode containing a std::share_ptr<std::map<std::string, std::shared_ptr<MetaDataNode>>>
         Implementation of MetaDataNode for a map containing other nodes (for tree data structure).
         * 
         */
        class MetaDataMapNode : public MetaDataNode{
            public:
                MetaDataMapNode();
                const MetaTypes getType() const {return MetaTypes::MAP;};
                /**
                 * @brief Add a new node to this map.
                 * 
                 * @param key Description of node
                 * @param value Node
                 */
                void addNode(std::string key, std::shared_ptr<MetaDataNode> value);
                /**
                 * @brief Delete node identified by key from map
                 * 
                 * @param key 
                 */
                void deleteNode(std::string key);
                /**
                 * @brief Copy all elements from this map to another map (deep-copy!)
                 * 
                 * @param copyMap 
                 */
                void copyToMap(std::shared_ptr<MetaDataMapNode> copyMap);
                /**
                 * @brief Get the a node identified by key id
                 * 
                 * @param key 
                 * @return std::shared_ptr<MetaDataNode> 
                 */
                std::shared_ptr<MetaDataNode> getNode(std::string key);
            private:
                /**
                 * @brief Internal implementation of map
                 * 
                 */
                std::map<std::string, std::shared_ptr<MetaDataNode>> m_values;
        };
        

    }
}
#endif