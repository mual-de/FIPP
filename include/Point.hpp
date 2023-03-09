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
    /**
     * @brief X value
     * 
     */
        T m_x;
        /**
         * @brief Y value
         * 
         */
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
        /**
         * @brief Get the absolute value (sqrt(x^2 + y^2)) of this point
         * 
         * @return double 
         */
        double getAbsValue();
        /**
         * @brief convert to human readable string
         * 
         * @return std::string 
         */
        inline std::string asStr() { return "x: " + std::to_string(m_x) + " - y: " + std::to_string(m_y); };
        /**
         * @brief get x/y values as yaml node 
         * 
         * @return YAML::Node 
         */
        YAML::Node asYaml();
        /**
         * @brief Get the product of x and y
         * 
         * @return T size of area spawned by a and y
         */
        inline T getArea() { return m_x * m_y; };
        /**
         * @brief get X Value
         * 
         * @return T value of x
         */
        inline T getX() { return m_x; };
        /**
         * @brief Get Y Value
         * 
         * @return T value of y
         */
        inline T getY() { return m_y; };
        /**
         * @brief get X Value as unsigned int
         * 
         * @return T value of x
         */
        inline T getXUInt() { return static_cast<unsigned int>(m_x); };
        /**
         * @brief Get Y Value as unsigned int
         * 
         * @return T value of y
         */
        inline T getYUInt() { return static_cast<unsigned int>(m_y); };
        /**
         * @brief Get value of x as int
         * 
         * @return int 
         */
        inline int getXInt() const { return static_cast<int>(m_x); };
        /**
         * @brief Get value of y as int
         * 
         * @return int 
         */
        inline int getYInt() const { return static_cast<int>(m_y); };
        /**
         * @brief Get value of x as float
         * 
         * @return float 
         */
        inline float getXFloat() const { return static_cast<float>(m_x); };
        /**
         * @brief Get value of y as float
         * 
         * @return float 
         */
        inline float getYFloat() const { return static_cast<float>(m_y); };
        /**
         * @brief Compare this point with another
         * 
         * @param cmp 
         * @return true x>cmp.x and y>cmp.y 
         * @return false 
         */
        inline bool operator>(Point<T> cmp) { return (m_x > cmp.m_x) && (m_y > cmp.m_y); };
        /**
         * @brief Compare this point with another
         * 
         * @param cmp 
         * @return true x<cmp.x and y<cmp.y 
         * @return false 
         */
        inline bool operator<(Point<T> cmp) { return (m_x < cmp.m_x) && (m_y < cmp.m_y); };
        /**
         * @brief Compare this point with another
         * 
         * @param cmp 
         * @return true x==cmp.x and y==cmp.y 
         * @return false 
         */
        inline bool operator==(Point<T> cmp) { return (m_x == cmp.m_x) && (m_y == cmp.m_y); };
        /**
         * @brief Compare this point with another
         * 
         * @param cmp 
         * @return true !(x==cmp.x and y==cmp.y) 
         * @return false 
         */
        inline bool operator!=(Point<T> cmp) { return !((m_x == cmp.m_x) && (m_y == cmp.m_y)); };
        /**
         * @brief Compare this point with another
         * 
         * @param cmp 
         * @return true x==cmp.x and y==cmp.y 
         * @return false 
         */
        template <typename U>
        inline bool operator==(Point<U> cmp) { return (m_x == static_cast<T>(cmp.m_x)) && (m_y == static_cast<T>(cmp.m_y)); };
        /**
         * @brief Compare this point with another
         * 
         * @param cmp 
         * @return true !(x==cmp.x and y==cmp.y) 
         * @return false 
         */
        template <typename U>
        inline bool operator!=(Point<U> cmp) { return !((m_x == static_cast<T>(cmp.m_x)) && (m_y == static_cast<T>(cmp.m_y))); };
        template <typename U>
        /**
         * @brief Divide point by divisor
         * 
         * @param div 
         * @return Point<T> x/div.x, y/div.y
         */
        inline Point<T> operator/(const U div) { return Point<T>(m_x / static_cast<T>(div), m_y / static_cast<T>(div)); };
        /**
         * @brief Divide point by divisor
         * 
         * @param div 
         * @return Point<T> x/div, y/div
         */
        inline Point<T> operator/(const T div) { return Point<T>(m_x / div, m_y / div); };
        template <typename U>
        /**
         * @brief Multiplicate point by multiplier
         * 
         * @param mul 
         * @return Point<T> x*mul.x, y*mul.y
         */
        inline Point<T> operator*(const U mul) { return Point<T>(m_x * static_cast<T>(mul), m_y * static_cast<T>(mul)); };
        /**
         * @brief Multiplicate point by multiplier
         * 
         * @param mul 
         * @return Point<T> x*mul, y*mul
         */
        inline Point<T> operator*(const T mul) { return Point<T>(m_x * mul, m_y * mul); };
        template <typename U>
        /**
         * @brief Add point to other point
         * 
         * @param add 
         * @return Point<T> x+other.x, y+other.y
         */
        inline Point<T> operator+(const Point<U> add) { return Point<T>(m_x + static_cast<T>(add.getX()), m_y + static_cast<T>(add.getY())); };
        /**
         * @brief Add point to other point
         * 
         * @param add 
         * @return Point<T> x+other.x, y+other.y
         */
        inline Point<T> operator+(const Point<T> add) { return Point<T>(m_x + add.m_x, m_y + add.m_y); };
        template <typename U>
        /**
         * @brief Sub point to other point
         * 
         * @param sub 
         * @return Point<T> x-other.x, y-other.y
         */
        inline Point<T> operator-(const Point<U> sub) { return Point<T>(m_x - static_cast<T>(sub.getX()), m_y - static_cast<T>(sub.getY())); };
        /**
         * @brief Sub point to other point
         * 
         * @param sub 
         * @return Point<T> x-other.x, y-other.y
         */
        inline Point<T> operator-(const Point<T> sub) { return Point<T>(m_x - sub.m_x , m_y - sub.m_y); };
    };

    /**
     * @brief Create a point from a given yaml node
     * 
     */
    namespace PointFactory{
    /**
     * @brief Get int Point from a given yaml node.
     * 
     * @param node 
     * @return Point<int> 
     */
        Point<int> ptIntFromYAML(YAML::Node node);
    };
};
#endif