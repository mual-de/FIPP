#ifndef __META_DATA_SYSTEM_HPP__
#define __META_DATA_SYSTEM_HPP__
#include <map>
#include <memory>
#include <vector>
#include "../Point.hpp"
#define BASIC_TYPES int, unsigned int, float, char, bool, Point<int>, Point<float>, Point<unsigned int>
#define VECTOR_TYPES std::vector<int>, std::vector<unsigned int>, std::vector<float>, std::vector<Point<int>>, std::vector<Point<float>>, std::vector<Point<unsigned int>>

namespace FIPP
{
    namespace img
    {

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

        class MetaDataValueNodeGeneric: public MetaDataNode{
            public:
                MetaDataValueNodeGeneric(MetaTypes type) : m_type(type){};
                const MetaTypes getType() const {return this->m_type;};
            protected:
                const MetaTypes m_type;
        };

        
        template <typename T>
        class MetaDataValueNode : public MetaDataValueNodeGeneric{
            public:
                MetaDataValueNode(T value);
                const T getValue() const{return this->m_value;};
            private:
                const T m_value;
        };

        template <typename T>
        class MetaDataVectorNode : public MetaDataValueNodeGeneric{
            public:
                MetaDataVectorNode(std::vector<T> vector);
                const std::vector<T> getValue() const {return this->m_values;};
                const std::vector<T>* getValuePtr() {return &this->m_values;};
            private:
                const std::vector<T> m_values;

        };

        class MetaDataMapNode : public MetaDataNode{
            public:
                MetaDataMapNode();
                const MetaTypes getType() const {return MetaTypes::MAP;};
                void addNode(std::string key, std::shared_ptr<MetaDataNode> value);
                void deleteNode(std::string key);
                void copyToMap(std::shared_ptr<MetaDataMapNode> copyMap);
                std::shared_ptr<MetaDataNode> getNode(std::string key);
            private:
                std::map<std::string, std::shared_ptr<MetaDataNode>> m_values;
        };
        

    }
}
#endif