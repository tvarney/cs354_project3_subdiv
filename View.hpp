#ifndef CS354_GENERIC_VIEW_HPP
#define CS354_GENERIC_VIEW_HPP

#define CS354_GENERIC_IMPL
#include "common.hpp"

#include <stdint.h>

namespace cs354 {
    struct Window;
    
    typedef void (*TimerFn_t)(int);
    
    enum MouseButton {
        MOUSE_BUTTON_LEFT = 1,
        MOUSE_BUTTON_RIGHT = 1 << 1,
        MOUSE_BUTTON_MIDDLE = 1 << 2
    };
    enum ButtonState {
        BUTTON_UP = 0,
        BUTTON_DOWN = 1
    };

#define ADJUST_GLUT_KEYCODE(code) ((code) << 16)
    extern const int KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5, KEY_F6;
    extern const int KEY_F7, KEY_F8, KEY_F9, KEY_F10, KEY_F11, KEY_F12;
    extern const int KEY_LEFT, KEY_UP, KEY_RIGHT, KEY_DOWN, KEY_PAGE_UP;
    extern const int KEY_PAGE_DOWN, KEY_HOME, KEY_END, KEY_INSERT;
    
    class View {
    public:
        virtual ~View();
        
        virtual void display() = 0;
        
        virtual void init() = 0;
        virtual void end() = 0;
        
        virtual void overlay() = 0;
        virtual void reshape(int width, int height) = 0;
        virtual void keyPressed(int ch) = 0;
        virtual void mousePressed(MouseButton button, ButtonState state) = 0;
        virtual void motion(int x, int y) = 0;
        virtual void passiveMotion(int x, int y) = 0;
        virtual void entry(int state) = 0;
        virtual void idle() = 0;
        
        void use();
        void postRedisplay();
        void timer(TimerFn_t timerfn, int value, int ms);
        
        static void InitViewSystem(int argc, char **argv, Window *window);
        static void EnterMainLoop(View &initial);
        static void EndViewSystem();
    protected:
        View();
    private:
        static View *CurrentView;
        static bool Init;
        
        static void DisplayCallback();
        static void OverlayCallback();
        static void ReshapeCallback(int width, int height);
        static void KeyboardCallback(unsigned char ch, int x, int y);
        static void SpecialCallback(int ch, int x, int y);
        static void MouseCallback(int button, int state, int x, int y);
        static void MotionCallback(int x, int y);
        static void PassiveMotionCallback(int x, int y);
        static void EntryCallback(int state);
        static void IdleCallback();
    };
    
    class BasicView : public View {
    public:
        virtual ~BasicView();
        
        virtual void init();
        virtual void end();
        virtual void overlay();
        virtual void reshape(int width, int height);
        virtual void keyPressed(int ch);
        virtual void mousePressed(MouseButton button, ButtonState state);
        virtual void motion(int x, int y);
        virtual void passiveMotion(int x, int y);
        virtual void entry(int state);
        virtual void idle();
    protected:
        BasicView();
        
        struct {
            struct {
                int x, y;
            } pos;
            uint32_t state;
            bool isInWindow;
        } mouse;
        
        struct {
            double aspect_ratio;
            struct {
                int width, height;
            } dim;
        } win;
    };
}

#endif
