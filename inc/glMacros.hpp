
#ifndef CS354_GENERIC_GL_MACROS_H
#define CS354_GENERIC_GL_MACROS_H

#include "Point.hpp"
#include "Vector.hpp"

namespace cs354 {
    void glVertex(const Point<float> point);
    void glVertex(const Point<double> point);
    void glNormal(const Vector<float> normal);
    void glNormal(const Vector<double> normal);
};

#endif
