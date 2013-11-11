
#include "Color.hpp"
#include <cmath>

using namespace cs354;

#define CLAMP(v, min, max) ((v) < (min) ? (min) : ((v) > (max) ? (max) : (v)))

ColorHSV::ColorHSV() : _h(0.0f), _s(0.0f), _v(0.0f), _a(1.0f) { }
ColorHSV::ColorHSV(float h, float s, float v, float a) :
    _h(CLAMP(h, 0.0f, 359.9999)), _s(CLAMP(s, 0.0f, 1.0f)),
    _v(CLAMP(v, 0.0f, 1.0f)), _a(CLAMP(a, 0.0f, 1.0f))
{ }
ColorHSV::ColorHSV(const ColorHSV &source) :
    _h(source._h), _s(source._s), _v(source._v), _a(source._a)
{ }
ColorHSV::~ColorHSV() { }

ColorHSV & ColorHSV::set(float h, float s, float v, float a) {
    _h = CLAMP(h, 0.0f, 359.9999f);
    _s = CLAMP(s, 0.0f, 1.0f);
    _v = CLAMP(v, 0.0f, 1.0f);
    _a = CLAMP(a, 0.0f, 1.0f);
    return *this;
}

float ColorHSV::hue() const {
    return _h;
}
float ColorHSV::saturation() const {
    return _s;
}
float ColorHSV::value() const {
    return _v;
}
float ColorHSV::alpha() const {
    return _a;
}
ColorHSV & ColorHSV::hue(float newvalue) {
    _h = CLAMP(newvalue, 0.0f, 359.9999);
    return *this;
}
ColorHSV & ColorHSV::saturation(float newvalue) {
    _s = CLAMP(newvalue, 0.0f, 1.0f);
    return *this;
}
ColorHSV & ColorHSV::value(float newvalue) {
    _v = CLAMP(newvalue, 0.0f, 1.0f);
    return *this;
}
ColorHSV & ColorHSV::alpha(float newvalue) {
    _a = CLAMP(newvalue, 0.0f, 1.0f);
    return *this;
}

bool ColorHSV::operator==(const ColorHSV hsv) {
    return (_h == hsv._h && _s == hsv._s && _v == hsv._v && _a == hsv._a);
}

ColorHSL::ColorHSL() : _h(0.0f), _s(0.0f), _l(0.0f), _a(1.0f) { }
ColorHSL::ColorHSL(float h, float s, float l, float a) :
    _h(CLAMP(h, 0.0f, 359.9999)), _s(CLAMP(s, 0.0f, 1.0f)),
    _l(CLAMP(l, 0.0f, 1.0f)), _a(CLAMP(a, 0.0f, 1.0f))
{ }
ColorHSL::ColorHSL(const ColorHSL &source) :
    _h(source._h), _s(source._s), _l(source._l), _a(source._a)
{ }
ColorHSL::~ColorHSL() { }

ColorHSL & ColorHSL::set(float h, float s, float l, float a) {
    _h = CLAMP(h, 0.0f, 359.9999f);
    _s = CLAMP(s, 0.0f, 1.0f);
    _l = CLAMP(l, 0.0f, 1.0f);
    _a = CLAMP(a, 0.0f, 1.0f);
    return *this;
}

float ColorHSL::hue() const {
    return _h;
}
float ColorHSL::saturation() const {
    return _s;
}
float ColorHSL::lightness() const {
    return _l;
}
float ColorHSL::alpha() const {
    return _a;
}

ColorHSL & ColorHSL::hue(float newvalue) {
    _h = CLAMP(newvalue, 0.0f, 359.9999);
    return *this;
}
ColorHSL & ColorHSL::saturation(float newvalue) {
    _s = CLAMP(newvalue, 0.0f, 1.0f);
    return *this;
}
ColorHSL & ColorHSL::lightness(float newvalue) {
    _l = CLAMP(newvalue, 0.0f, 1.0f);
    return *this;
}
ColorHSL & ColorHSL::alpha(float newvalue) {
    _a = CLAMP(newvalue, 0.0f, 1.0f);
    return *this;
}

bool ColorHSL::operator==(const ColorHSL hsl) {
    return (_h == hsl._h && _s == hsl._s && _l == hsl._l && _a == hsl._a);
}

ColorHCL::ColorHCL() : _h(0.0f), _c(0.0f), _l(0.0f), _a(1.0f) { }
ColorHCL::ColorHCL(float h, float c, float l, float a) :
    _h(CLAMP(h, 0.0f, 359.9999)), _c(CLAMP(c, 0.0f, 1.0f)),
    _l(CLAMP(l, 0.0f, 1.0f)), _a(CLAMP(a, 0.0f, 1.0f))
{ }
ColorHCL::ColorHCL(const ColorHCL &source) :
    _h(source._h), _c(source._c), _l(source._l), _a(source._a)
{ }
ColorHCL::~ColorHCL() { }

ColorHCL & ColorHCL::set(float h, float c, float l, float a) {
    _h = CLAMP(h, 0.0f, 359.9999f);
    _c = CLAMP(c, 0.0f, 1.0f);
    _l = CLAMP(l, 0.0f, 1.0f);
    _a = CLAMP(a, 0.0f, 1.0f);
    return *this;
}

float ColorHCL::hue() const {
    return _h;
}
float ColorHCL::chroma() const {
    return _c;
}
float ColorHCL::lightness() const {
    return _l;
}
float ColorHCL::alpha() const {
    return _a;
}

ColorHCL & ColorHCL::hue(float newvalue) {
    _h = CLAMP(newvalue, 0.0f, 359.9999f);
    return *this;
}
ColorHCL & ColorHCL::chroma(float newvalue) {
    _c = CLAMP(newvalue, 0.0f, 1.0f);
    return *this;
}
ColorHCL & ColorHCL::lightness(float newvalue) {
    _l = CLAMP(newvalue, 0.0f, 1.0f);
    return *this;
}
ColorHCL & ColorHCL::alpha(float newvalue) {
    _a = CLAMP(newvalue, 0.0f, 1.0f);
    return *this;
}

bool ColorHCL::operator==(const ColorHCL hcl) {
    return (_h == hcl._h && _c == hcl._c && _l == hcl._l && _a == hcl._a);
}

Color::Color() : _r(0.0f), _g(0.0f), _b(0.0f), _a(1.0f) { }
Color::Color(float r, float g, float b, float a) :
    _r(CLAMP(r, 0.0f, 1.0f)), _g(CLAMP(g, 0.0f, 1.0f)),
    _b(CLAMP(b, 0.0f, 1.0f)), _a(CLAMP(a, 0.0f, 1.0f))
{ }
Color::Color(const Color &source) :
    _r(source._r), _g(source._g), _b(source._b), _a(source._a)
{ }
Color::~Color() { }

Color & Color::set(float r, float g, float b, float a) {
    _r = CLAMP(r, 0.0f, 1.0f);
    _g = CLAMP(g, 0.0f, 1.0f);
    _b = CLAMP(b, 0.0f, 1.0f);
    _a = CLAMP(a, 0.0f, 1.0f);
    return *this;
}

float Color::red() const {
    return _r;
}
float Color::green() const {
    return _g;
}
float Color::blue() const {
    return _b;
}
float Color::alpha() const {
    return _a;
}

Color & Color::red(float newvalue) {
    _r = CLAMP(newvalue, 0.0f, 1.0f);
    return *this;
}
Color & Color::green(float newvalue) {
    _g = CLAMP(newvalue, 0.0f, 1.0f);
    return *this;
}
Color & Color::blue(float newvalue) {
    _b = CLAMP(newvalue, 0.0f, 1.0f);
    return *this;
}
Color & Color::alpha(float newvalue) {
    _a = CLAMP(newvalue, 0.0f, 1.0f);
    return *this;
}

bool Color::operator==(const Color rgb) {
    return (_r == rgb._r && _g == rgb._g && _b == rgb._b && _a == rgb._a);
}

#define MIN3(x, y, z) \
    ((y) <= (z) ? ((x) <= (y) ? (x) : (y)) : ((x) <= (z) ? (x) : (z)))
#define MAX3(x, y, z) \
    ((y) >= (z) ? ((x) >= (y) ? (x) : (y)) : ((x) >= (z) ? (x) : (z)))

Color Color::ConvertHSV(const ColorHSV hsv) {
    Color result;
    result._a = hsv.alpha();
    
    float chroma = hsv.value() * hsv.saturation();
    float h = hsv.hue() * 0.016666666666666666;
    float x = chroma * (1 - std::fabs(std::fmod(h, 2.0f) - 1));
    float m = hsv.value() - chroma;
    switch(int(h)) {
    case 0:
        result._r = chroma;
        result._g = x;
        break;
    case 1:
        result._r = x;
        result._g = chroma;
        break;
    case 2:
        result._g = chroma;
        result._b = x;
        break;
    case 3:
        result._g = x;
        result._b = chroma;
        break;
    case 4:
        result._r = x;
        result._b = chroma;
        break;
    case 5:
        result._r = chroma;
        result._b = x;
        break;
    default:
        return Color();
    }
    result._r += m;
    result._g += m;
    result._b += m;
    return result;
}
Color Color::ConvertHSL(const ColorHSL hsl) {
    Color result;
    result._a = hsl.alpha();
    
    float chroma =
        1 - std::fabs(2.0f * hsl.lightness() - 1.0f) * hsl.saturation();
    float h = hsl.hue() * 0.016666666666666666;
    float x = chroma * (1.0f - std::fabs(std::fmod(h, 2.0f) - 1.0f));
    float m = hsl.lightness() - 0.5 * chroma;
    switch(int(h)) {
    case 0:
        result._r = chroma;
        result._g = x;
        break;
    case 1:
        result._r = x;
        result._g = chroma;
        break;
    case 2:
        result._g = chroma;
        result._b = x;
        break;
    case 3:
        result._g = x;
        result._b = chroma;
        break;
    case 4:
        result._r = x;
        result._b = chroma;
        break;
    case 5:
        result._r = chroma;
        result._b = x;
        break;
    default:
        return Color();
    }
    result._r += m;
    result._g += m;
    result._b += m;
    return result;
}
Color Color::ConvertHCL(const ColorHCL hcl) {
    Color result;
    result._a = hcl.alpha();
    float h = hcl.hue() * 0.016666666666666666;
    float x = hcl.chroma() * (1.0f - std::fabs(std::fmod(h, 2.0f) - 1));
    switch(int(h)) {
    case 0:
        result._r = hcl.chroma();
        result._g = x;
        break;
    case 1:
        result._r = x;
        result._g = hcl.chroma();
        break;
    case 2:
        result._g = hcl.chroma();
        result._b = x;
        break;
    case 3:
        result._g = x;
        result._b = hcl.chroma();
        break;
    case 4:
        result._r = x;
        result._b = hcl.chroma();
        break;
    case 5:
        result._r = hcl.chroma();
        result._b = x;
        break;
    default:
        return Color();
    }
    float cl = (0.3 * result._r + 0.59 * result._g + 0.11 * result._b);
    float m = hcl.lightness() - cl;
    result._r += m;
    result._g += m;
    result._b += m;
    return result;
}

const Color Color::White(1.0f, 1.0f, 1.0f);
const Color Color::Silver(0.75f, 0.75f, 0.75f);
const Color Color::Gray(0.5f, 0.5f, 0.5f);
const Color Color::Black(0.0f, 0.0f, 0.0f);
const Color Color::Red(1.0f, 0.0f, 0.0f);
const Color Color::Maroon(0.5f, 0.0f, 0.0f);
const Color Color::Yellow(1.0f, 1.0f, 0.0f);
const Color Color::Olive(0.5f, 0.5f, 0.0f);
const Color Color::Lime(0.0f, 1.0f, 0.0f);
const Color Color::Green(0.0f, 0.5f, 0.0f);
const Color Color::Aqua(0.0f, 1.0f, 1.0f);
const Color Color::Teal(0.0f, 0.5f, 0.5f);
const Color Color::Blue(0.0f, 0.0f, 1.0f);
const Color Color::Navy(0.0f, 0.0f, 0.5f);
const Color Color::Fuchsia(1.0f, 0.0f, 1.0f);
const Color Color::Purple(0.5f, 0.0f, 0.5f);
