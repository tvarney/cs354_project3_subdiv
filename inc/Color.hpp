
#ifndef CS354_GENERIC_COLOR_HPP
#define CS354_GENERIC_COLOR_HPP

#include <stdint.h>

namespace cs354 {
    class ColorHSV {
    public:
        ColorHSV();
        ColorHSV(float h, float s, float v, float a = 1.0f);
        ColorHSV(const ColorHSV &source);
        ~ColorHSV();
        
        ColorHSV & set(float h, float s, float v, float a = 1.0f);
        
        float hue() const;
        float saturation() const;
        float value() const;
        float alpha() const;
        
        ColorHSV & hue(float newvalue);
        ColorHSV & saturation(float newvalue);
        ColorHSV & value(float newvalue);
        ColorHSV & alpha(float newvalue);
        
        bool operator==(const ColorHSV hsv);
    private:
        float _h, _s, _v, _a;
    };
    
    class ColorHSL {
    public:
        ColorHSL();
        ColorHSL(float h, float s, float l, float a = 1.0f);
        ColorHSL(const ColorHSL &source);
        ~ColorHSL();
        
        ColorHSL & set(float h, float s, float l, float a = 1.0f);
        
        float hue() const;
        float saturation() const;
        float lightness() const;
        float alpha() const;
        
        ColorHSL & hue(float newvalue);
        ColorHSL & saturation(float newvalue);
        ColorHSL & lightness(float newvalue);
        ColorHSL & alpha(float newvalue);
        
        bool operator==(const ColorHSL hsl);
    private:
        float _h, _s, _l, _a;
    };
    
    class ColorHCL {
    public:
        ColorHCL();
        ColorHCL(float h, float c, float l, float a = 1.0f);
        ColorHCL(const ColorHCL &source);
        ~ColorHCL();
        
        ColorHCL & set(float h, float c, float l, float a = 1.0f);
        
        float hue() const;
        float chroma() const;
        float lightness() const;
        float alpha() const;
        
        ColorHCL & hue(float newvalue);
        ColorHCL & chroma(float newvalue);
        ColorHCL & lightness(float newvalue);
        ColorHCL & alpha(float newvalue);
        
        bool operator==(const ColorHCL hcl);
    private:
        float _h, _c, _l, _a;
    };
    
    class Color {
    public:
        Color();
        Color(float r, float g, float b, float a = 1.0f);
        Color(const Color &source);
        ~Color();
        
        Color & set(float r, float g, float b, float a = 1.0f);
        
        float red() const;
        float green() const;
        float blue() const;
        float alpha() const;
        
        Color & red(float newvalue);
        Color & green(float newvalue);
        Color & blue(float newvalue);
        Color & alpha(float newvalue);
        
        bool operator==(const Color rgb);
        
        static Color ConvertHSV(const ColorHSV hsv);
        static Color ConvertHSL(const ColorHSL hsl);
        static Color ConvertHCL(const ColorHCL hcl);
        
        /* Standard Web Colors */
        static const Color White, Silver, Gray, Black, Red, Maroon, Yellow;
        static const Color Olive, Lime, Green, Aqua, Teal, Blue, Navy;
        static const Color Fuchsia, Purple;
    private:
        float _r, _g, _b, _a;
    };
}

#endif
