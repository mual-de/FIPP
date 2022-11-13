#include "ImageContainer/MetaDataSystem.hpp"
using namespace FIPP;
using namespace FIPP::img;

template <>
MetaDataValueNode<int>::MetaDataValueNode(int value) : m_value(value), MetaDataValueNodeGeneric(MetaTypes::INT){};
template <>
MetaDataValueNode<unsigned int>::MetaDataValueNode(unsigned int value) : m_value(value), MetaDataValueNodeGeneric(MetaTypes::UNSIGNED_INT){};
template <>
MetaDataValueNode<float>::MetaDataValueNode(float value) : m_value(value), MetaDataValueNodeGeneric(MetaTypes::FLOAT){};
template <>
MetaDataValueNode<char>::MetaDataValueNode(char value) : m_value(value), MetaDataValueNodeGeneric(MetaTypes::CHAR){};
template <>
MetaDataValueNode<bool>::MetaDataValueNode(bool value) : m_value(value), MetaDataValueNodeGeneric(MetaTypes::BOOL){};
template <>
MetaDataValueNode<Point<int>>::MetaDataValueNode(Point<int> value) : m_value(value), MetaDataValueNodeGeneric(MetaTypes::POINT_INT){};
template <>
MetaDataValueNode<Point<float>>::MetaDataValueNode(Point<float> value) : m_value(value), MetaDataValueNodeGeneric(MetaTypes::POINT_FLOAT){};
template <>
MetaDataValueNode<Point<unsigned int>>::MetaDataValueNode(Point<unsigned int> value) : m_value(value), MetaDataValueNodeGeneric(MetaTypes::UNSIGNED_INT){};
template <>
MetaDataValueNode<std::string>::MetaDataValueNode(std::string value) : m_value(value), MetaDataValueNodeGeneric(MetaTypes::STRING){};

template <>
MetaDataVectorNode<int>::MetaDataVectorNode(std::vector<int> vector) : m_values(vector), MetaDataValueNodeGeneric(MetaTypes::VECTOR_INT){};
template <>
MetaDataVectorNode<unsigned int>::MetaDataVectorNode(std::vector<unsigned int> vector) : m_values(vector), MetaDataValueNodeGeneric(MetaTypes::VECTOR_UNSIGNED_INT){};
template <>
MetaDataVectorNode<float>::MetaDataVectorNode(std::vector<float> vector) : m_values(vector), MetaDataValueNodeGeneric(MetaTypes::VECTOR_FLOAT){};
template <>
MetaDataVectorNode<char>::MetaDataVectorNode(std::vector<char> vector) : m_values(vector), MetaDataValueNodeGeneric(MetaTypes::VECTOR_CHAR){};
template <>
MetaDataVectorNode<bool>::MetaDataVectorNode(std::vector<bool> vector) : m_values(vector), MetaDataValueNodeGeneric(MetaTypes::VECTOR_BOOL){};
template <>
MetaDataVectorNode<Point<int>>::MetaDataVectorNode(std::vector<FIPP::Point<int>> vector) : m_values(vector), MetaDataValueNodeGeneric(MetaTypes::VECTOR_POINT_INT){};
template <>
MetaDataVectorNode<Point<float>>::MetaDataVectorNode(std::vector<Point<float>> vector) : m_values(vector), MetaDataValueNodeGeneric(MetaTypes::VECTOR_POINT_FLOAT){};
template <>
MetaDataVectorNode<Point<unsigned int>>::MetaDataVectorNode(std::vector<Point<unsigned int>> vector) : m_values(vector), MetaDataValueNodeGeneric(MetaTypes::VECTOR_UNSIGNED_INT){};
template <>
MetaDataVectorNode<std::string>::MetaDataVectorNode(std::vector<std::string> vector) : m_values(vector), MetaDataValueNodeGeneric(MetaTypes::VECTOR_STRING){};

MetaDataMapNode::MetaDataMapNode()
{
}

void MetaDataMapNode::addNode(std::string key, std::shared_ptr<MetaDataNode> value)
{
    this->m_values.insert(std::make_pair(key, value));
}
void MetaDataMapNode::deleteNode(std::string key)
{
    this->m_values.erase(key);
}
void MetaDataMapNode::copyToMap(std::shared_ptr<MetaDataMapNode> copyMap)
{
    std::map<std::string, std::shared_ptr<MetaDataNode>>::iterator it;
    for (it = this->m_values.begin(); it != this->m_values.end(); it++)
    {
        // Maps can be edited (added/deleted informations so they need to be unique for every metadataset)
        if (dynamic_cast<MetaDataMapNode *>(it->second.get()))
        {
            std::shared_ptr<MetaDataMapNode> newSubMapPtr = std::make_shared<MetaDataMapNode>();
            std::shared_ptr<MetaDataMapNode> oldMap = std::dynamic_pointer_cast<MetaDataMapNode>(it->second);
            oldMap->copyToMap(newSubMapPtr);
            copyMap->addNode(it->first, newSubMapPtr);
            // all other values are only linked to the copyMap tree
        }
        else
        {
            copyMap->addNode(it->first, it->second);
        }
    }
}

std::shared_ptr<MetaDataNode> MetaDataMapNode::getNode(std::string key)
{
    if (this->m_values.find(key) != this->m_values.end())
    {
        return this->m_values.at(key);
    }
    return nullptr;
}
