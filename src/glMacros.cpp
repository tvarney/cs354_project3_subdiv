
#include "glMacros.hpp"
#include "common.hpp"

using namespace cs354;

void cs354::glVertex(const Point<float> point) {
    glVertex3f(point.x, point.y, point.z);
}
void cs354::glVertex(const Point<double> point) {
    glVertex3d(point.x, point.y, point.z);
}
void cs354::glNormal(const Vector<float> normal) {
    glNormal3f(normal.x, normal.y, normal.z);
}
void cs354::glNormal(const Vector<double> normal) {
    glNormal3d(normal.x, normal.y, normal.z);
}
