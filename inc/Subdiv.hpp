
#ifndef CS354_PROJECT3_SUBDIV_HPP
#define CS354_PROJECT3_SUBDIV_HPP

#include <vector>
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
        
        float scale, rotation_y, rotation_z;
        unsigned int vertical, horizontal;
        int display_mode;
        
        size_t npoints;
        Point3f *points;
        std::vector<unsigned int> elements;
        std::vector<Vector3f> normals;
        unsigned int display_list;
    };
    
    /* Handles to our two views */
    extern BasicView *draw, *display;
}

#endif
