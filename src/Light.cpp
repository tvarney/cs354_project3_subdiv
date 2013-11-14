
#include "Light.hpp"

#include <iostream>
#include "common.hpp"

using namespace cs354;

Light::Light() {
    setAmbient(Color::Silver);
    setDiffuse(Color::Silver);
    setSpecular(Color::White);
    setDirection(Vector3f(0.0f, 1.0f, 0.0f));
}
Light::Light(const Vector3f direction, const Color ambient,
             const Color diffuse, const Color specular)
{
    setAmbient(ambient);
    setDiffuse(diffuse);
    setSpecular(specular);
    setDirection(direction);
}
Light::Light(const Point3f position, const Color ambient,
             const Color diffuse, const Color specular)
{
    setAmbient(ambient);
    setDiffuse(diffuse);
    setSpecular(specular);
    setPosition(position);
}
Light::~Light() { }

Light & Light::setAmbient(const Color source) {
    ambient[0] = source.red();
    ambient[1] = source.green();
    ambient[2] = source.blue();
    ambient[3] = 1.0f;
    return *this;
}
Light & Light::setDiffuse(const Color source) {
    diffuse[0] = source.red();
    diffuse[1] = source.green();
    diffuse[2] = source.blue();
    diffuse[3] = 1.0f;
    return *this;
}
Light & Light::setSpecular(const Color source) {
    specular[0] = source.red();
    specular[1] = source.green();
    specular[2] = source.blue();
    specular[3] = 1.0f;
    return *this;
}

Light & Light::setDirection(const Vector3f source) {
    pos[0] = source.x;
    pos[1] = source.y;
    pos[2] = source.z;
    pos[3] = 0.0f;
    return *this;
}
Light & Light::setPosition(const Point3f source) {
    pos[0] = source.x;
    pos[1] = source.y;
    pos[2] = source.z;
    pos[3] = 1.0f;
    return *this;
}

void Light::use(uint32_t lnum) const {
    glLightfv(GL_LIGHT0, GL_POSITION, pos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
}

uint32_t Light::LightEnum(uint32_t num) {
    return GL_LIGHT0 + num;
}
