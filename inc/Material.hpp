
#ifndef CS354_GENERIC_MATERIAL_HPP
#define CS354_GENERIC_MATERIAL_HPP

#include "Color.hpp"

namespace cs354 {
    enum Face {
        FACE_FRONT,
        FACE_BACK,
    };
    
    struct Material {
        float ambient[4], diffuse[4], specular[4];
        float shininess;
        
        Material();
        Material(const Color &ambient, const Color &diffuse,
                 const Color &specular, float shininess);
        Material(const Material &source);
        ~Material();
        
        Material & operator=(const Material &rhs);
        
        void use() const;
        void use(Face face) const;
        
        static const Material Brass, Bronze, Chrome, Copper, Gold, Silver;
        static const Material Plastic, Rubber;
        static const Material Red;
    };
}

#endif
