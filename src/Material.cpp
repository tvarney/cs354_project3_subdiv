
#include "Material.hpp"

#include "common.hpp"

using namespace cs354;

#define CLAMP(value, min, max) \
    ((value) < (min) ? (min) : ((value) > (max) ? (max) : (value)))

Material::Material() {
    ambient[0] = ambient[1] = ambient[2] = 0.5;
    ambient[3] = 1.0;
    diffuse[0] = diffuse[1] = diffuse[2] = 0.7;
    diffuse[3] = 1.0;
    specular[0] = specular[1] = specular[2] = 0.9;
    specular[3] = 1.0;
    shininess = 5.0;
}
Material::Material(const Color &ambient, const Color &diffuse,
                   const Color &specular, float shininess)
{
    this->ambient[0] = ambient.red();
    this->ambient[1] = ambient.green();
    this->ambient[2] = ambient.blue();
    this->ambient[3] = ambient.alpha();
    
    this->diffuse[0] = diffuse.red();
    this->diffuse[1] = diffuse.green();
    this->diffuse[2] = diffuse.blue();
    this->diffuse[3] = diffuse.alpha();
    
    this->specular[0] = specular.red();
    this->specular[1] = specular.green();
    this->specular[2] = specular.blue();
    this->specular[3] = specular.alpha();
    
    this->shininess = CLAMP(shininess, 0.0, 128.0);
}

Material::Material(const Material &source) {
    ambient[0] = source.ambient[0];
    ambient[1] = source.ambient[1];
    ambient[2] = source.ambient[2];
    ambient[3] = source.ambient[3];

    diffuse[0] = source.diffuse[0];
    diffuse[1] = source.diffuse[1];
    diffuse[2] = source.diffuse[2];
    diffuse[3] = source.diffuse[3];

    specular[0] = source.specular[0];
    specular[1] = source.specular[1];
    specular[2] = source.specular[2];
    specular[3] = source.specular[3];
    
    shininess = CLAMP(source.shininess, 0.0, 128.0);
}

Material::~Material() { }

Material & Material::operator=(const Material &rhs) {
    ambient[0] = rhs.ambient[0];
    ambient[1] = rhs.ambient[1];
    ambient[2] = rhs.ambient[2];
    ambient[3] = rhs.ambient[3];
    
    diffuse[0] = rhs.diffuse[0];
    diffuse[1] = rhs.diffuse[1];
    diffuse[2] = rhs.diffuse[2];
    diffuse[3] = rhs.diffuse[3];
    
    specular[0] = rhs.specular[0];
    specular[1] = rhs.specular[1];
    specular[2] = rhs.specular[2];
    specular[3] = rhs.specular[3];
    
    shininess = rhs.shininess;
    return *this;
}

void Material::use() const {
    glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
    glMaterialf(GL_FRONT, GL_SHININESS, shininess);
}
void Material::use(Face face) const {
    GLenum glface = (face == FACE_FRONT ? GL_FRONT : GL_BACK);
    glMaterialfv(glface, GL_AMBIENT, ambient);
    glMaterialfv(glface, GL_DIFFUSE, diffuse);
    glMaterialfv(glface, GL_SPECULAR, specular);
    glMaterialf(glface, GL_SHININESS, shininess);
}

const Material Material::Sapphire(Color(0.0215,  0.0215,  0.1745),
                                  Color(0.07568, 0.07568, 0.61424),
                                  Color(0.633,   0.633,   0.727811),
                                  77);
const Material Material::Ruby(Color(0.1745, 0.01175, 0.01175),
                              Color(0.61424, 0.04136, 0.04136),
                              Color(0.727811, 0.626959, 0.626959),
                              77);
const Material Material::Brass(Color(0.329412, 0.223529, 0.027451),
                               Color(0.780392, 0.568627, 0.113725),
                               Color(0.992157, 0.941176, 0.807843),
                               0.21794872 * 128);
const Material Material::Bronze(Color(0.2125, 0.1275, 0.054),
                                Color(0.714, 0.4284, 0.18144),
                                Color(0.393548, 0.271906, 0.166721),
                                0.2 * 128);
const Material Material::Chrome(Color(0.25, 0.25, 0.25),
                                Color(0.4, 0.4, 0.4),
                                Color(0.774597, 0.774597, 0.774597),
                                0.6 * 128);
const Material Material::Copper(Color(0.19125, 0.0725, 0.0225),
                                Color(0.7038, 0.27048, 0.0828),
                                Color(0.256777, 0.137622, 0.086014),
                                0.1 * 128);
const Material Material::Gold(Color(0.24725, 0.1995, 0.0745),
                              Color(0.75164, 0.60648, 0.22648),
                              Color(0.628281, 0.555802, 0.366065),
                              0.4 * 128);
const Material Material::Silver(Color(0.19225, 0.19225, 0.19225),
                                Color(0.50754, 0.50754, 0.50754),
                                Color(0.508273, 0.508273, 0.508273),
                                0.4 * 128);
const Material Material::Plastic(Color(0.2, 0.2, 0.2),
                                 Color(0.55, 0.55, 0.55),
                                 Color(0.7, 0.7, 0.7),
                                 0.25 * 128);
const Material Material::Rubber(Color(0.05, 0.0, 0.0),
                                Color(0.5, 0.4, 0.4),
                                Color(0.7, 0.4, 0.4),
                                0.078125 * 128);

const Material Material::Red(Color(0.5, 0.0, 0.0),
                             Color(0.7, 0.0, 0.0),
                             Color(0.9, 0.0, 0.0),
                             1.0f);
