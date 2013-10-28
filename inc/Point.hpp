
#ifndef CS354_GENERIC_POINT_HPP
#define CS354_GENERIC_POINT_HPP

#include <ostream>
#include "Triple.hpp"

namespace cs354 {
    template <typename T>
    struct Point {
        T x, y, z;
        
        Point() { }
        Point(T x, T y, T z) : x(x), y(y), z(z) { }
        Point(T data[3]) : x(data[0]), y(data[1]), z(data[2]) { }
        Point(const Point<T> &source) :
            x(source.x), y(source.y), z(source.z)
        { }
        ~Point() { }
        
        static const Point<T> Origin;
    };
    
    template <typename T>
    const Point<T> Point<T>::Origin(0, 0, 0);
    
    typedef Point<float> Point3f;
    typedef Point<double> Point3d;
    typedef Point<int> Point3i;
    typedef Point<unsigned int> Point3ui;
    
    template <typename T>
    std::ostream & operator<<(std::ostream &out, Point<T> &rhs) {
        out << "(" << rhs.x << ", " << rhs.y << ", " << rhs.z << ")";
        return out;
    }
}

#endif
