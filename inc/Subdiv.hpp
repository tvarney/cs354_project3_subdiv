
#ifndef CS354_PROJECT3_SUBDIV_HPP
#define CS354_PROJECT3_SUBDIV_HPP

#include <vector>
#include "Light.hpp"
#include "Material.hpp"
#include "Model.hpp"
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
        
        virtual void init();
        virtual void end();
        virtual void display();
        
        virtual void keyPressed(int ch);
        virtual void motion(int x, int y);
        
        void make_model(const std::vector<Point3f> &control_points);
    protected:
        float scale, rotation_y, rotation_z, rotspeed;
        unsigned int vertical, horizontal;
        
        DisplayMode dispmode;
        ShadeType shadetype;
        
        Light light;
        Model *model;
        
        /* Material variables */
        bool flat;
        Material mat;
        uint32_t matid;
        std::vector<const Material *> mats;
    };
    
    /* Handles to our two views */
    extern BasicView *draw, *display;
}

#endif
