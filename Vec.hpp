
#ifndef CS354_GENERIC_VECTOR_HPP
#define CS354_GENERIC_VECTOR_HPP

#include <cmath>
#include <ostream>

#include "Triple.hpp"

namespace cs354 {
    template <typename T>
    struct Vector {
        union {
            Triple<T> coord;
            T data[3];
        };
        
        Vector() { }
        Vector(T x, T y, T z) :
            coord(x, y, z)
        { }
        Vector(const T data[3]) :
            coord(data[0], data[1], data[2])
        { }
        Vector(const Vector<T> &source) :
            coord(source.data[0], source.data[1], source.data[2])
        { }
        ~Vector() { }
        
        Vector<T> operator+(const Vector &rhs) const {
            return Vector<T>(data[0] + rhs.data[0], data[1] + rhs.data[1],
                             data[2] + rhs.data[2]);
        }
        Vector<T> operator-(const Vector &rhs) const {
            return Vector<T>(data[0] - rhs.data[0], data[1] - rhs.data[1],
                             data[2] + rhs.data[2]);
        }
        Vector<T> operator*(const Vector &rhs) const {
            return Vector<T>(data[1] * rhs.data[2] - data[2] * rhs.data[1],
                             data[0] * rhs.data[2] - data[2] * rhs.data[0],
                             data[0] * rhs.data[1] - data[1] * rhs.data[0]);
        }
        Vector<T> operator/(const Vector &rhs) const {
            return Vector<T>(data[1] / rhs.data[2] - data[2] / rhs.data[1],
                             data[0] / rhs.data[2] - data[2] / rhs.data[0],
                             data[0] / rhs.data[1] - data[1] / rhs.data[0]);
        }
        Vector<T> operator*(T scalar) const {
            return Vector<T>(data[0]*scalar, data[1]*scalar, data[2]*scalar);
        }
        Vector<T> operator/(T scalar) const {
            return Vector<T>(data[0]/scalar, data[1]/scalar, data[2]/scalar);
        }
        
        Vector<T> inverse() const {
            return Vector<T>(((T)1.0)/data[0], ((T)1.0)/data[1],
                             ((T)1.0)/data[2]);
        }
        Vector<T> normalize() const {
            return *this / magnitude();
        }
        Vector<T> shuffle(int xs, int ys, int zs) const {
            return Vector<T>(data[xs], data[ys], data[zs]);
        }
        
        T dot(const Vector &rhs) const {
            return data[0]*rhs.data[0] + data[1]*rhs.data[1] +
                data[2]*rhs.data[2];
        }
        T magsquared() const {
            return data[0]*data[0] + data[1]*data[1] + data[2]*data[2];
        }
        T magnitude() const {
            return std::sqrt(magsquared());
        }
        
        Vector<T> & operator=(const Vector<T> &rhs) {
            data[0] = rhs.data[0];
            data[1] = rhs.data[1];
            data[2] = rhs.data[2];
            return *this;
        }
        Vector<T> & operator+=(const Vector<T> &rhs) {
            return *this = *this + rhs;
        }
        Vector<T> & operator-=(const Vector<T> &rhs) {
            return *this = *this - rhs;
        }
        Vector<T> & operator*=(const Vector<T> &rhs) {
            return *this = *this * rhs;
        }
        Vector<T> & operator/=(const Vector<T> &rhs) {
            return *this = *this / rhs;
        }
        Vector<T> & operator*=(T scalar) {
            return *this = *this * scalar;
        }
        Vector<T> & operator/=(T scalar) {
            return *this = *this / scalar;
        }
    };
    
    typedef Vector<double> Vector3d;
    typedef Vector<float> Vector3f;
    
    template <typename T>
    std::ostream & operator<<(std::ostream &out, const Vector<T> &vec) {
        out << "<" << vec.data[0] << ", " << vec.data[1] << ", " <<
            vec.data[2] << ">";
        return out;
    }
}

#endif
