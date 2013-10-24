
#ifndef CS354_PROJECT3_SUBDIV_HPP
#define CS354_PROJECT3_SUBDIV_HPP

#include "View.hpp"

#include <vector>

namespace cs354 {
    struct Point {
        float x,y;
        Point();
        Point(float x, float y);
        Point(const Point &source);
        ~Point();
    };
    
    float DistSquared(Point &p1, Point &p2);
    
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
        Point calcMousePos();
        
        std::vector<Point> points;
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
        
        void process(std::vector<Point> &points);
    protected:
        float scale;
    };
    
    /* Handles to our two views */
    extern BasicView *draw, *display;
}

#endif
