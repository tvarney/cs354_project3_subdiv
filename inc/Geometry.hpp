
#ifndef CS354_GENERIC_GEOMETRY_HPP
#define CS354_GENERIC_GEOMETRY_HPP

#include "Matrix.hpp"
#include "Point.hpp"
#include "Vector.hpp"

namespace cs354 {
    template <typename T>
    Vector<T> operator-(const Point<T> rhs, const Point<T> lhs) {
        return Vector<T>(rhs.x - lhs.x, rhs.y - lhs.y, rhs.z - lhs.z);
    }
    template <typename T>
    Point<T> operator+(const Point<T> rhs, const Vector<T> lhs) {
        return Point<T>(rhs.x + lhs.x, rhs.y + lhs.y, rhs.z + lhs.z);
    }
    template <typename T>
    Point<T> operator+(const Vector<T> rhs, const Point<T> lhs) {
        return Point<T>(rhs.x + lhs.x, rhs.y + lhs.y, rhs.z + lhs.z);
    }
    
    template <typename T>
    Vector<T> operator*(const Matrix<T> &rhs, const Vector<T> lhs) {
        return Vector<T>(lhs);
    }
    template <typename T>
    Point<T> operator*(const Matrix<T> &rhs, const Point<T> lhs) {
        return Point<T>(lhs);
    }
    
    /* Hopefully the compiler recognizes the floating point versions can be
     * optimized to a multiply instead of a div.
     */
    template <typename T>
    Point<T> lerp(const Point<T> p1, const Point<T> p2) {
        return Point<T>(p1.x + p2.x, p1.y + p2.y, p1.z + p2.z) / ((T)2);
    }
    template <typename T>
    Vector<T> lerp(const Vector<T> v1, const Vector<T> v2) {
        return (v1 + v2) / ((T)2);
    }
}

#endif
