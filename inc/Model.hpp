
#ifndef CS354_PROJECT3_MODEL_HPP
#define CS354_PROJECT3_MODEL_HPP

#include <stdint.h>
#include <vector>
#include "Point.hpp"
#include "Vector.hpp"

namespace cs354 {
    struct Material;
    
    enum Direction {
        HORIZONTAL,
        VERTICAL,
    };
    enum DisplayMode {
        DISPLAY_POINTS,
        DISPLAY_WIRES,
        DISPLAY_MODEL
    };
    enum ShadeType {
        SHADE_GOURAUD,
        SHADE_PHONG,
    };
    
    class Model {
    public:
        Model(const std::vector<Point3f> & control_points);
        ~Model();
        
        void display(DisplayMode mode, ShadeType type);
        void subdivide(Direction dir);
        void setMaterial(const Material &material);
        int depth() const;
        void depth(int newdepth);
    protected:
        void create_face(uint32_t p1, uint32_t p2, uint32_t p3);
        void update();
        void subdiv_h();
        void subdiv_v();
        
        void display_points();
        void display_wire();
        void display_gouraud();
        void display_phong();
        
        void r_subdiv_face(int n, Point3f p1, Point3f p2, Point3f p3,
                           Vector3f n1, Vector3f n2, Vector3f n3);
        
        uint32_t display_list;
        size_t width, height, size;
        Point3f *points;
        Vector3f *normals;
        uint32_t *nfaces;
        std::vector<uint32_t> elements;
        const Material *material;
        DisplayMode lastmode;
        ShadeType lasttype;
        int _depth;
    };
}

#endif
