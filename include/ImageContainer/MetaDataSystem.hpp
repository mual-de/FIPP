#ifndef __META_DATA_SYSTEM_HPP__
#define __META_DATA_SYSTEM_HPP__
#include <variant>
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
            VECTOR_INT,
            VECTOR_UNSIGNED_INT,
            VECTOR_FLOAT,
            STRING,
            VECTOR_POINT_INT,
            VECTOR_POINT_FLOAT,
            VECTOR_POINT_UNSIGNED_INT,
            VECTOR_STRING,
            MAP,
            NONE
        } MetaTypes;

        class MetaDataNode
        {
        public:
            virtual MetaTypes getType() = 0;
        };

        class MetaDataMap : public MetaDataNode
        {
        public:
            MetaDataMap();
            void addValue<
            const std::map<std::string, MetaDataNode> *getMapPtr() const
            {
                return &m_data;
            };
            MetaTypes getType() { return MetaTypes::MAP; };
            std::map<std::string, MetaDataNode> getMap() { return m_data; };

        private:
            std::map<std::string, MetaDataNode> m_data;
        };

        template <typename T>
        class MetaDataVec : public MetaDataNode
        {
        public:
            MetaDataVec(std::vector<T> vec);
            void addValue(T val) { this->m_vec.push_back(val); };
            const std::vector<T> *getVectorPtr() { return &this->m_vec; };
            std::vector<T> getVector() { return this->m_vec; };
            MetaTypes getType() { return this->m_type; };

        private:
            std::vector<T> m_vec;
            const m_type;
        };

        template <typename T>
        class MetaDataVal : public MetaDataNode
        {
        public:
            MetaDataVal<T>(T val);
            T getValue() { return this->m_val; };
            MetaTypes getType() { return this->m_meta; };

        private:
            const T m_val;
            const MetaTypes m_meta;
        };

        MetaDataVal<int>(int val) : m_val(val), m_type(MetaTypes::INT){};
        MetaDataVal<float>(float val) : m_val(val), m_type(MetaTypes::FLOAT){};
        MetaDataVal<bool>(bool val) : m_val(val), m_type(MetaTypes::BOOL){};
        MetaDataVal<unsigned int>(unsigned int val) : m_val(val), m_type(MetaTypes::UNSIGNED_INT){};
        MetaDataVal<char>(char val) : m_val(val), m_type(MetaTypes::CHAR){};
        MetaDataVal<Point<int>>(Point<int> val) : m_val(val), m_type(MetaTypes::POINT_INT){};
        MetaDataVal<Point<float>>(Point<float> val) : m_val(val), m_type(MetaTypes::POINT_FLOAT){};
        MetaDataVal<Point<unsigned int>>(Point<unsigned int> val) : m_val(val), m_type(MetaTypes::POINT_UNSIGNED_INT){};
        MetaDataVal<std::string>(std::string val) : m_val(val), m_type(MetaTypes::STRING){};

        MetaDataVec<int>() : m_type(MetaTypes::VECTOR_INT){};
        MetaDataVec<float>() : m_type(MetaTypes::VECTOR_FLOAT){};
        MetaDataVec<unsigned int>() : m_type(MetaTypes::VECTOR_UNSIGNED_INT){};
        MetaDataVec<Point<int>>() : m_type(MetaTypes::VECTOR_POINT_INT){};
        MetaDataVec<Point<float>>() : m_type(MetaTypes::VECTOR_POINT_FLOAT){};
        MetaDataVec<Point<unsigned int>>() : m_type(MetaTypes::VECTOR_POINT_UNSIGNED_INT){};
        MetaDataVec<std::string>() : m_type(MetaTypes::VECTOR_STRING){};

    }
}
#endif