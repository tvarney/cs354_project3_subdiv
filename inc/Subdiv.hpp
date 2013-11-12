
#ifndef CS354_PROJECT3_SUBDIV_HPP
#define CS354_PROJECT3_SUBDIV_HPP

#include <vector>
#include "Material.hpp"
#include "Point.hpp"
#include "Vector.hpp"
#include "View.hpp"

namespace cs354 {
    class DrawView : public BasicView {
    public:
        DrawView();
        ~DrawView();
        
        virtual void display();
        virtual void init();
        virtual void end();
        
        virtual void keyPressed(int ch);
        virtual void mousePressed(MouseButton button, ButtonState state);
        virtual void motion(int x, int y);
    protected:
        Point3f calcMousePos();
        
        std::vector<Point3f> points;
        bool mouse_mode, snap_mode;
    };
    
    class DisplayView : public BasicView {
    public:
        DisplayView();
        ~DisplayView();
        
        virtual void display();
        virtual void init();
        virtual void end();
        
        virtual void keyPressed(int ch);
        
        void process(std::vector<Point3f> &points);
    protected:
        void display_points();
        void display_wire();
        void display_gouraud();
        void display_phong();
        
        void create_face(uint32_t p1, uint32_t p2, uint32_t p3);
        void update_model();
        
        void subdivide_horiz();
        void subdivide_vertical();
        
        float scale, rotation_y, rotation_z;
        unsigned int vertical, horizontal;
        int display_mode;
        
        float lightpos[4];
        
        size_t npoints;
        Point3f *points;
        Vector3f *normals;
        std::vector<uint32_t> elements;
        uint32_t matid;
        std::vector<const Material *> mats;
        unsigned int display_list;
    };
    
    /* Handles to our two views */
    extern BasicView *draw, *display;
}

#endif
