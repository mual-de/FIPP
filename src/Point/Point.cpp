#include "Point.hpp"
#include "math.h"
#include <yaml-cpp/yaml.h>
using namespace FIPP;

template <typename T>
Point<T>::Point(T x, T y)
{
    this->m_x = x;
    this->m_y = y;
}
/**
 * @brief Calculates the absolute value (pythagoras/euclid distance) of a point
 *
 * @tparam T
 * @return T
 */
template <typename T>
double Point<T>::getAbsValue()
{
    return sqrt(this->m_x * this->m_x + this->m_y * this->m_y);
}

template <typename T>
YAML::Node Point<T>::asYaml()
{
    YAML::Node node;
    node["x"] = m_x;
    node["y"] = m_y;
    return node;
};

Point<int> PointFactory::ptIntFromYAML(YAML::Node node)
{
    return Point<int>(node["x"].as<int>(), node["y"].as<int>());
}

template class Point<int>;
template class Point<unsigned int>;
template class Point<float>;
template class Point<double>;
template class Point<unsigned char>;
template class Point<char>;
