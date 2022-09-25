#include "Point.hpp"
#include "math.h"
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