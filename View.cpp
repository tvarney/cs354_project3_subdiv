
#include "View.hpp"

#include <stddef.h>
#include "common.hpp"
#include "Window.hpp"

using namespace cs354;

#define ADJUST_GLUT_KEYCODE(code) ((code) << 16)
const int cs354::KEY_F1 = ADJUST_GLUT_KEYCODE(GLUT_KEY_F1);
const int cs354::KEY_F2 = ADJUST_GLUT_KEYCODE(GLUT_KEY_F2);
const int cs354::KEY_F3 = ADJUST_GLUT_KEYCODE(GLUT_KEY_F3);
const int cs354::KEY_F4 = ADJUST_GLUT_KEYCODE(GLUT_KEY_F4);
const int cs354::KEY_F5 = ADJUST_GLUT_KEYCODE(GLUT_KEY_F5);
const int cs354::KEY_F6 = ADJUST_GLUT_KEYCODE(GLUT_KEY_F6);
const int cs354::KEY_F7 = ADJUST_GLUT_KEYCODE(GLUT_KEY_F7);
const int cs354::KEY_F8 = ADJUST_GLUT_KEYCODE(GLUT_KEY_F8);
const int cs354::KEY_F9 = ADJUST_GLUT_KEYCODE(GLUT_KEY_F9);
const int cs354::KEY_F10 = ADJUST_GLUT_KEYCODE(GLUT_KEY_F10);
const int cs354::KEY_F11 = ADJUST_GLUT_KEYCODE(GLUT_KEY_F11);
const int cs354::KEY_F12 = ADJUST_GLUT_KEYCODE(GLUT_KEY_F12);
const int cs354::KEY_LEFT = ADJUST_GLUT_KEYCODE(GLUT_KEY_LEFT);
const int cs354::KEY_UP = ADJUST_GLUT_KEYCODE(GLUT_KEY_UP);
const int cs354::KEY_RIGHT = ADJUST_GLUT_KEYCODE(GLUT_KEY_RIGHT);
const int cs354::KEY_DOWN = ADJUST_GLUT_KEYCODE(GLUT_KEY_DOWN);
const int cs354::KEY_PAGE_UP = ADJUST_GLUT_KEYCODE(GLUT_KEY_PAGE_UP);
const int cs354::KEY_PAGE_DOWN = ADJUST_GLUT_KEYCODE(GLUT_KEY_PAGE_DOWN);
const int cs354::KEY_HOME = ADJUST_GLUT_KEYCODE(GLUT_KEY_HOME);
const int cs354::KEY_END = ADJUST_GLUT_KEYCODE(GLUT_KEY_END);
const int cs354::KEY_INSERT = ADJUST_GLUT_KEYCODE(GLUT_KEY_INSERT);

View * View::CurrentView = NULL;
bool View::Init = false;

View::View() { }
View::~View() { }

void View::use() {
    if(View::CurrentView) {
        View::CurrentView->end();
    }
    View::CurrentView = this;
    init();
}
void View::postRedisplay() {
    glutPostRedisplay();
}

void View::timer(TimerFn_t timerfn, int value, int ms) {
    glutTimerFunc(ms, timerfn, value);
}

void View::InitViewSystem(int argc, char **argv, Window *window) {
    if(!View::Init) {
        glutInit(&argc, argv);
        const char *title = "";
        if(window != NULL) {
            title = window->title.c_str();
            glutInitWindowSize(window->width, window->height);
            glutInitWindowPosition(window->x, window->y);
            glutInitDisplayMode(window->mode);
        }
        glutCreateWindow(title);
    }
    
    glutDisplayFunc(View::DisplayCallback);
    glutOverlayDisplayFunc(View::OverlayCallback);
    glutReshapeFunc(View::ReshapeCallback);
    glutKeyboardFunc(View::KeyboardCallback);
    glutSpecialFunc(View::SpecialCallback);
    glutMouseFunc(View::MouseCallback);
    glutMotionFunc(View::MotionCallback);
    glutPassiveMotionFunc(View::PassiveMotionCallback);
    glutEntryFunc(View::EntryCallback);
    glutIdleFunc(View::IdleCallback);
}

void View::EnterMainLoop(View &initial) {
    if(View::CurrentView == NULL) {
        initial.use();
        glutMainLoop();
    }else {
        initial.use();
    }
}

void View::EndViewSystem() {
    View::CurrentView = NULL;
    View::Init = false;
    
    glutDisplayFunc(NULL);
    glutOverlayDisplayFunc(NULL);
    glutReshapeFunc(NULL);
    glutKeyboardFunc(NULL);
    glutSpecialFunc(NULL);
    glutMouseFunc(NULL);
    glutMotionFunc(NULL);
    glutPassiveMotionFunc(NULL);
    glutEntryFunc(NULL);
    glutIdleFunc(NULL);
}

void View::DisplayCallback() {
    if(View::CurrentView) {
        View::CurrentView->display();
        
        glFlush();
        glutSwapBuffers();
    }
}
void View::OverlayCallback() {
    if(View::CurrentView) {
        View::CurrentView->overlay();
    }
}
void View::ReshapeCallback(int width, int height) {
    if(View::CurrentView) {
        View::CurrentView->reshape(width, height);
    }
}
void View::KeyboardCallback(unsigned char ch, int x, int y) {
    if(View::CurrentView) {
        View::CurrentView->keyPressed(ch);
    }
}
void View::SpecialCallback(int ch, int x, int y) {
    if(View::CurrentView) {
        View::CurrentView->keyPressed(ADJUST_GLUT_KEYCODE(ch));
    }
}
void View::MouseCallback(int button, int state, int x, int y) {
    if(View::CurrentView) {
        MouseButton mButton = MOUSE_BUTTON_LEFT;
        ButtonState mState = (state == GLUT_UP ? BUTTON_UP : BUTTON_DOWN);
        switch(button) {
        case GLUT_LEFT_BUTTON:
            button = MOUSE_BUTTON_LEFT;
            break;
        case GLUT_RIGHT_BUTTON:
            button = MOUSE_BUTTON_RIGHT;
            break;
        case GLUT_MIDDLE_BUTTON:
            button = MOUSE_BUTTON_MIDDLE;
            break;
        default:
            return;
        }
        
        View::CurrentView->mousePressed(mButton, mState);
    }
}
void View::MotionCallback(int x, int y) {
    if(View::CurrentView) {
        View::CurrentView->motion(x, y);
    }
}
void View::PassiveMotionCallback(int x, int y) {
    if(View::CurrentView) {
        View::CurrentView->passiveMotion(x, y);
    }
}
void View::EntryCallback(int state) {
    if(View::CurrentView) {
        View::CurrentView->entry(state);
    }
}
void View::IdleCallback() {
    if(View::CurrentView) {
        View::CurrentView->idle();
    }
}

BasicView::BasicView() {
    win.dim.width = glutGet(GLUT_WINDOW_WIDTH);
    win.dim.height = glutGet(GLUT_WINDOW_HEIGHT);
    win.aspect_ratio = double(win.dim.width) / double(win.dim.height);
}
BasicView::~BasicView() { }

/* Stubbed functions */
void BasicView::init() { }
void BasicView::end() { }
void BasicView::overlay() { }
void BasicView::reshape(int width, int height) {
    win.aspect_ratio = (double(width) / double(height));
    win.dim.width = width;
    win.dim.height = height;
}
void BasicView::keyPressed(int ch) { }
void BasicView::idle() { }

/* Implemented functions  */
void BasicView::mousePressed(MouseButton button, ButtonState state) {
    uint32_t bmask = (uint32_t)button;
    if(state == BUTTON_UP) {
        bmask = ~bmask;
    }
    mouse.state |= bmask;
}

void BasicView::passiveMotion(int x, int y) {
    mouse.pos.x = x;
    mouse.pos.y = y;
}

void BasicView::motion(int x, int y) {
    mouse.pos.x = x;
    mouse.pos.y = y;
}

void BasicView::entry(int state) {
    mouse.isInWindow = (state = GLUT_ENTERED);
}
