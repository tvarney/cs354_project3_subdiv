
#include "Subdiv.hpp"

#include <iostream>
#include "Geometry.hpp"

using namespace cs354;

DrawView::DrawView() : mouse_mode(false), snap_mode(true) { }
DrawView::~DrawView() {
    points.clear();
}

void DrawView::display() {
    glDisable(GL_LIGHTING);
    
    glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glViewport(0,0,win.dim.width,win.dim.height);
    
    float mid_x = float(win.dim.width) * 0.5;
    float mid_y = float(win.dim.height) * 0.5;
    
    glDisable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-mid_x, mid_x, mid_y, -mid_y, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    glLineWidth(2.0);
    glPointSize(3.0);
    
    glBegin(GL_LINES); {
        glColor3f(0, 0.5, 0);
        glVertex2f(0, -mid_y);
        glVertex2f(0, mid_y);
        
        glColor3f(0, 0, 0.5);
        glVertex2f(-mid_x, 0);
        glVertex2f(mid_x, 0);
    } glEnd();
    
    size_t i, size;
    size = points.size();
    
    if(size >= 1) {
        Point3f mpos(calcMousePos());
        glColor3f(0.4,0,0);
        glBegin(GL_LINE_STRIP);
        for(i = 0; i < size; ++i) {
            glVertex2f(points[i].x, points[i].y);
        }
        if(mouse.state & MOUSE_BUTTON_LEFT) {
            glVertex2f(mpos.x, mpos.y);
        }
        glEnd();
        glBegin(GL_POINTS);
        glColor3f(0.8,0,0);
        if(mouse.state & MOUSE_BUTTON_LEFT) {
            for(i = 0; i < size; ++i) {
                glVertex2f(points[i].x, points[i].y);
            }
            glColor3f(0.9,0.9,0.9);
            glVertex2f(mpos.x, mpos.y);
        }else {
            for(i = 0; i < size - 1; ++i) {
                glVertex2f(points[i].x, points[i].y);
            }
            glColor3f(0.9, 0.9, 0.9);
            glVertex2f(points[size - 1].x, points[size - 1].y);
        }
    }
    glEnd();
}
void DrawView::init() { }
void DrawView::end() { }

void DrawView::keyPressed(int ch) {
    switch(ch) {
    case 'q':
        std::exit(0);
        break;
    case '/':
    case '?':
    case KEY_RIGHT:
    case KEY_LEFT:
        reinterpret_cast<DisplayView *>(cs354::display)->make_model(points);
        View::SetCurrent(*(cs354::display));
        break;
    case 'm':
        mouse_mode = !mouse_mode;
        std::cout << "Mouse Mode: " << (mouse_mode ? "Remove Closest" :
                                        "Remove last") << std::endl;
        break;
    case 's':
        snap_mode = !snap_mode;
        std::cout << "Snap Mode: " <<
            (snap_mode ? "Snap To Axis" : "Off") << std::endl;
        break;
    }
}

void DrawView::mousePressed(MouseButton button, ButtonState state) {
    BasicView::mousePressed(button, state);
    
    if(state == BUTTON_UP) {
        Point3f mousepos(calcMousePos());
        switch(button) {
        case MOUSE_BUTTON_LEFT:
            std::cout << "Adding Point [" << points.size() << "] (" <<
                mousepos.x << ", " << mousepos.y << ")" << std::endl;
            points.push_back(mousepos);
            View::PostRedisplay();
            break;
        case MOUSE_BUTTON_RIGHT:
            if(points.size() == 0) {
                break;
            }
            if(mouse_mode) {
                size_t index = 0;
                float next;
                float closest = (mousepos - points[0]).magsquared();
                for(size_t i = 1; i < points.size(); ++i) {
                    next = (mousepos - points[i]).magsquared();
                    if(next < closest) {
                        index = i;
                        closest = next;
                    }
                }
                points.erase(points.begin()+index);
            }else {
                points.pop_back();
            }
            
            View::PostRedisplay();
            break;
        default:
            break;
        }
    }
}

void DrawView::motion(int x, int y) {
    BasicView::motion(x, y);
    if(mouse.state & MOUSE_BUTTON_LEFT) {
        View::PostRedisplay();
    }
}

Point3f DrawView::calcMousePos() {
    int x = mouse.pos.x - win.dim.width / 2;
    int y = mouse.pos.y - win.dim.height / 2;
    if(snap_mode) {
        if(x < 5 && x > -5) {
            x = 0;
        }
        if(y < 5 && y > -5) {
            y = 0;
        }
    }
    return Point3f(x, y, 0);
}

