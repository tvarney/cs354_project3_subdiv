
#include "Subdiv.hpp"

#include <iostream>
#include <cstdlib>

using namespace cs354;

BasicView *cs354::draw = NULL, *cs354::display = NULL;

Point::Point() : x(0), y(0) { }
Point::Point(float x, float y) : x(x), y(y) { }
Point::Point(const Point &source) : x(source.x), y(source.y) { }
Point::~Point() { }

float cs354::DistSquared(Point &p1, Point &p2) {
    Point p3(p2.x - p1.x, p2.y - p1.y);
    return p3.x*p3.x + p3.y*p3.y;
}

DrawView::DrawView() : mouse_mode(false) { }
DrawView::~DrawView() { }

void DrawView::display() {
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
        glColor3f(0, 0.3, 0);
        glVertex2f(0, -mid_y);
        glVertex2f(0, mid_y);
        
        glColor3f(0, 0, 0.3);
        glVertex2f(-mid_x, 0);
        glVertex2f(mid_x, 0);
    } glEnd();
    
    size_t i, size;
    size = points.size();
    
    if(size >= 1) {
        glColor3f(0.4,0,0);
        glBegin(GL_LINE_STRIP);
        for(i = 0; i < size; ++i) {
            glVertex2f(points[i].x, points[i].y);
        }
        glEnd();
        glBegin(GL_POINTS);
        glColor3f(0.8,0,0);
        for(i = 0; i < size - 1; ++i) {
            glVertex2f(points[i].x, points[i].y);
        }
        glColor3f(0.9, 0.9, 0.9);
        glVertex2f(points[size - 1].x, points[size - 1].y);
    }
    glEnd();
}
void DrawView::init() { }
void DrawView::end() { }

void DrawView::keyPressed(int ch) {
    std::cout << "Keycode: " << ch << std::endl;
    switch(ch) {
    case 'q':
        std::exit(0);
        break;
    case '/':
    case '?':
    case KEY_RIGHT:
    case KEY_LEFT:
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
    if(state == BUTTON_DOWN) {
        Point mousepos(calcMousePos());
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
                float closest = DistSquared(mousepos, points[0]);
                for(size_t i = 1; i < points.size(); ++i) {
                    next = DistSquared(mousepos, points[i]);
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

Point DrawView::calcMousePos() {
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
    return Point(x, y);
}

DisplayView::DisplayView() { }
DisplayView::~DisplayView() { }

void DisplayView::display() {
    glClearColor(1,1,1,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void DisplayView::init() { }
void DisplayView::end() { }

void DisplayView::keyPressed(int ch) {
    View::SetCurrent(*(cs354::draw));
}
