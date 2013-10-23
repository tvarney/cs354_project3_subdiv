
#ifndef CS354_PROJECT3_SUBDIV_HPP
#define CS354_PROJECT3_SUBDIV_HPP

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
    protected:
        
    };
    
    class DisplayView : public BasicView {
    public:
        DisplayView();
        ~DisplayView();
        
        virtual void display();
        virtual void init();
        virtual void end();
        
        virtual void keyPressed(int ch);
    protected:
        
    };
    
    /* Handles to our two views */
    extern BasicView *draw, *display;
}

#endif
