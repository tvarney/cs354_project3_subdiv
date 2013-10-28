
#include "Subdiv.hpp"

#include <cmath>
#include <cstdlib>
#include <iostream>
#include "Geometry.hpp"

using namespace cs354;

BasicView *cs354::draw = NULL, *cs354::display = NULL;

DrawView::DrawView() : mouse_mode(false) { }
DrawView::~DrawView() {
    points.clear();
}

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
        View::SetCurrent(*(cs354::display));
        reinterpret_cast<DisplayView *>(cs354::display)->process(points);
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

#define DISPLAY_MODE_POINTS 0
#define DISPLAY_MODE_WIRE 1
#define DISPLAY_MODE_GOURAUD 2
#define DISPLAY_MODE_PHONG 3

DisplayView::DisplayView() :
    scale(1.0f), rotation_y(0.0f), rotation_z(0.0f), vertical(0),
    horizontal(0), display_mode(DISPLAY_MODE_GOURAUD), npoints(0),
    points(NULL), elements(), display_list(0)
{ }
DisplayView::~DisplayView() {
    end();
}

void DisplayView::display() {
    glViewport(0,0,win.dim.width, win.dim.height);
    
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    
    glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, win.aspect_ratio, 1.0, 101.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 0, 0,
              0, 0, 15.0,
              0, 1, 0);
    
    switch(display_mode) {
    case DISPLAY_MODE_POINTS:
        display_points();
        return;
    case DISPLAY_MODE_WIRE:
        display_wire();
        return;
    case DISPLAY_MODE_GOURAUD:
        display_gouraud();
        return;
    case DISPLAY_MODE_PHONG:
        display_phong();
        return;
    default:
        std::cerr << "Invalid 3D display mode: " << display_mode << std::endl;
        std::cerr << "Switching to default Gouraud shading" << std::endl;
        display_mode = DISPLAY_MODE_GOURAUD;
        display_gouraud();
        return;
    }
}
void DisplayView::init() {
    /* Set the default display parameters */
    scale = 1.0f;
    rotation_y = 0.0f;
    rotation_z = 0.0f;
    vertical = 0;
    horizontal = 0;
    display_mode = DISPLAY_MODE_GOURAUD;
}
void DisplayView::end() {
    if(display_list != 0) {
        /*TODO: delete the display list */
        display_list = 0;
    }
    
    if(points != NULL) {
        delete [] points;
        elements.clear();
        normals.clear();
        points = NULL;
    }
}

void DisplayView::keyPressed(int ch) {
    /* I would like to dispatch a worker thread to do the computation, and draw
     * a 'working' indicator while the worker is not done. This gets rid of the
     * problems from the previous project where the executable would freeze
     * while computing a large thing. The display list creation has to be done
     * on the main thread, so that is left to the display function.
     */
    
    switch(ch) {
    case 'q':
        std::exit(0);
        break;
    default:
        View::SetCurrent(*(cs354::draw));
        return;
    }
}

/* Constant value representing 120 degrees in radians; found using python,
 * >>> deg120 = (120.0 / 360.0) * (math.pi * 2)
 * >>> cos120 = math.cos(deg120)
 * >>> sin120 = math.sin(deg120)
 * There is no need to compute a constant value every time, nor is there a need
 * to do a full Matrix Multiply for a simple rotation that is known ahead of
 * time.
 */
#define COS_120 -0.5
#define SIN_120 0.8660254037844387

void DisplayView::process(std::vector<Point3f> &points) {
    init();
    if(this->points != NULL) {
        std::cout << "Error!" << std::endl;
    }
    float r;
    /* Technically unsafe; should check if points.size() > size_t(-1)/3 */
    size_t num = points.size();
    if(num <= 1) {
        View::SetCurrent(*(cs354::draw));
        return;
    }
    
    npoints = num * 3;
    Point3f *newpoints = new Point3f[npoints];
    this->points = newpoints;
    unsigned int p[6];
    Vector3f vec1, vec2, norm;
    
    r = points[0].x;
    newpoints[0] = Point3f(points[0]);
    newpoints[num] = Point3f(r * -SIN_120, points[0].y, r * COS_120);
    newpoints[num*2] = Point3f(r * -SIN_120, points[0].y, r * -COS_120);
    /* Compute the new points, then the new quads (split into triangles), then
     * the new face normals. Per point normals can be computed if desired from
     * the face normals. */
    for(size_t i = 1; i < num; ++i) {
        r = points[i].x;
        /* New points */
        newpoints[i] = Point3f(points[i]);
        newpoints[num+i] = Point3f(r * -SIN_120, points[i].y, r * COS_120);
        newpoints[num*2+i] = Point3f(r * -SIN_120, points[i].y, r * -COS_120);
        /* Element insertion; 6 triangles, 18 elements */
        p[0] = i - 1;
        p[1] = i;
        p[2] = num + i - 1;
        p[3] = num + i;
        p[4] = 2 * num + i - 1;
        p[5] = 2 * num + i;
        
        elements.push_back(p[0]);
        elements.push_back(p[3]);
        elements.push_back(p[1]);
        
        elements.push_back(p[0]);
        elements.push_back(p[2]);
        elements.push_back(p[3]);
        
        elements.push_back(p[2]);
        elements.push_back(p[5]);
        elements.push_back(p[3]);
        
        elements.push_back(p[2]);
        elements.push_back(p[4]);
        elements.push_back(p[5]);
        
        elements.push_back(p[4]);
        elements.push_back(p[0]);
        elements.push_back(p[1]);
        
        elements.push_back(p[4]);
        elements.push_back(p[1]);
        elements.push_back(p[5]);
        
        /* Face Normal calculations:
         * The computed normals are pushed twice since there are two triangles
         * in each quad.
         */
        vec1 = (newpoints[p[3]] - newpoints[p[0]]);
        vec2 = (newpoints[p[1]] - newpoints[p[0]]);
        norm = (vec1 * vec2).normalize();
        normals.push_back(norm);
        normals.push_back(norm);
        
        vec1 = (newpoints[p[5]] - newpoints[p[2]]);
        vec2 = (newpoints[p[3]] - newpoints[p[2]]);
        norm = (vec1 * vec2).normalize();
        normals.push_back(norm);
        normals.push_back(norm);
        
        vec1 = (newpoints[p[0]] - newpoints[p[4]]);
        vec2 = (newpoints[p[1]] - newpoints[p[4]]);
        norm = (vec1 * vec2).normalize();
        normals.push_back(norm);
        normals.push_back(norm);
    }
    
    for(size_t i = 0; i < npoints; ++i) {
        std::cout << "Point " << i << " = " << newpoints[i] << std::endl;
    }
    for(size_t i = 0; i < elements.size(); ++i) {
        std::cout << "Element " << i << " = " << elements[i] << std::endl;
    }
}

void DisplayView::display_points() {
    std::cout << "Displaying Points" << std::endl;
    size_t npoints = this->npoints;
    glBegin(GL_POINTS); {
        for(size_t i = 0; i < npoints; ++i) {
            glVertex3f(points[i].x, points[i].y, points[i].z);
        }
    }glEnd();
}
/* This function has a LOT of overdraw -. - */
static Point3f dummy;
void DisplayView::display_wire() {
    std::cout << "Displaying Wireframe" << std::endl;
    
    size_t nelements = elements.size();
    std::cout << "  Displaying " << nelements / 3 << " triangles" << std::endl;
    Point3f &p1 = dummy, &p2 = dummy, &p3 = dummy;
    glBegin(GL_LINES); {
        for(size_t i = 0; i < nelements; i += 3) {
            p1 = points[elements[i]];
            p2 = points[elements[i + 1]];
            p3 = points[elements[i + 2]];
            glVertex3f(p1.x, p1.y, p1.z);
            glVertex3f(p2.x, p2.y, p2.z);
            glVertex3f(p2.x, p2.y, p2.z);
            glVertex3f(p3.x, p3.y, p3.z);
            glVertex3f(p3.x, p3.y, p3.z);
            glVertex3f(p1.x, p1.y, p1.z);
        }
    }glEnd();
}
void DisplayView::display_gouraud() {
    std::cout << "Displaying Gouraud" << std::endl;
    size_t nelements = elements.size() / 3;
    Point3f &p1 = dummy, &p2 = dummy, &p3 = dummy;
    glColor3f(1.0, 1.0, 1.0);
    glBegin(GL_TRIANGLES); {
        for(size_t i = 0; i < nelements; i += 1) {
            p1 = points[elements[i*3]];
            p2 = points[elements[i*3 + 1]];
            p3 = points[elements[i*3 + 2]];
            glNormal3f(normals[i].x, normals[i].y, normals[i].z);
            glVertex3f(p1.x, p1.y, p1.z);
            glVertex3f(p2.x, p2.y, p2.z);
            glVertex3f(p3.x, p3.y, p3.z);
        }
    }glEnd();
}
void DisplayView::display_phong() {
    std::cout << "Display: Phong not implemented" << std::endl;
}
