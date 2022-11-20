/**
 * @file Point.hpp
 * @author Alexander Mueller (dev@alexandermaxmueller.de)
 * @brief Type Definition for 2-Dimension Point type (also used for 2-D dimensions etc.)
 * @version 0.1
 * @date 2022-09-24
 *
 * @copyright Copyright (c) 2022
 *
 * Template Type for 2-D Points.
 */
#ifndef __POINT_HPP__
#define __POINT_HPP__
#include <string>

namespace YAML
{
    class Node;
};

namespace FIPP
{
    template <typename T>
    class Point
    {
    public:
        T m_x;
        T m_y;
        Point<T>() : m_x(static_cast<T>(0)), m_y(static_cast<T>(0)){};
        Point<T>(T x, T y);
        template <typename U>
        Point<T>(U x, U y)
        {
            m_x = static_cast<T>(x);
            m_y = static_cast<T>(y);
        };
        template <typename U>
        Point<T>(Point<U> pt)
        {
            m_x = static_cast<T>(pt.m_x);
            m_y = static_cast<T>(pt.m_y);
        };
        double getAbsValue();
        inline std::string asStr() { return "x: " + std::to_string(m_x) + " - y: " + std::to_string(m_y); };
        YAML::Node asYaml();
        inline T getArea() { return m_x * m_y; };
        inline T getX() { return m_x; };
        inline T getY() { return m_y; };
        inline int getXInt() const { return static_cast<int>(m_x); };
        inline int getYInt() const { return static_cast<int>(m_y); };
        inline float getXFloat() const { return static_cast<float>(m_x); };
        inline float getYFloat() const { return static_cast<float>(m_y); };
        inline bool operator>(Point<T> cmp) { return (m_x > cmp.m_x) && (m_y > cmp.m_y); };
        inline bool operator<(Point<T> cmp) { return (m_x < cmp.m_x) && (m_y < cmp.m_y); };
        inline bool operator==(Point<T> cmp) { return (m_x == cmp.m_x) && (m_y == cmp.m_y); };
        inline bool operator!=(Point<T> cmp) { return !((m_x == cmp.m_x) && (m_y == cmp.m_y)); };
        template <typename U>
        inline bool operator==(Point<U> cmp) { return (m_x == static_cast<T>(cmp.m_x)) && (m_y == static_cast<T>(cmp.m_y)); };
        template <typename U>
        inline bool operator!=(Point<U> cmp) { return !((m_x == static_cast<T>(cmp.m_x)) && (m_y == static_cast<T>(cmp.m_y))); };
    };

    class PointFactory
    {
    public:
        static Point<int> ptIntFromYAML(YAML::Node node);
    };
};
#endif