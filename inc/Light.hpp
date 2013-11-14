
#ifndef CS354_PROJECT3_LIGHT_HPP
#define CS354_PROJECT3_LIGHT_HPP

#include <stdint.h>
#include "Color.hpp"
#include "Point.hpp"
#include "Vector.hpp"

namespace cs354 {
    class Light {
    public:
        Light();
        Light(const Vector3f direction, const Color ambient = Color::Silver,
              const Color diffuse = Color::Silver,
              const Color specular = Color::White);
        Light(const Point3f position, const Color ambient = Color::Silver,
              const Color diffuse = Color::Silver,
              const Color specular = Color::White);
        ~Light();
        
        Light & setAmbient(const Color source);
        Light & setDiffuse(const Color source);
        Light & setSpecular(const Color source);
        Light & setDirection(const Vector3f source);
        Light & setPosition(const Point3f source);
        
        void use(uint32_t lnum) const;
        
        static uint32_t LightEnum(uint32_t num);
    protected:
        float ambient[4], diffuse[4], specular[4], pos[4];
    };
}

#endif
