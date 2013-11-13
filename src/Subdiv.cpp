
#define CS354_GENERIC_IMPL
#include "Subdiv.hpp"

#include <cmath>
#include <cstdlib>
#include <iostream>
#include "Geometry.hpp"
#include "Material.hpp"

using namespace cs354;

#define DELETE(ptr) do { if((ptr) != NULL) { delete[] ptr; ptr = NULL; } }while(0)

BasicView *cs354::draw = NULL, *cs354::display = NULL;

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

static double _dist = 51.0;

DisplayView::DisplayView() :
    scale(0.1f), rotation_y(0.0f), rotation_z(0.0f), vertical(0),
    horizontal(0), display_mode(DISPLAY_MODE_GOURAUD), npoints(0),
    points(NULL), normals(NULL), elements(), matid(0), display_list(0)
{
    mats.push_back(&(Material::Bronze));
    mats.push_back(&(Material::Silver));
    mats.push_back(&(Material::Gold));
    mats.push_back(&(Material::Plastic));
}
DisplayView::~DisplayView() {
    end();
}

void DisplayView::display() {
    glViewport(0,0,win.dim.width, win.dim.height);
    glLineWidth(1.0f);
    glPointSize(1.0f);
    
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    
    glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, win.aspect_ratio, 1.0, 100.0);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 0, _dist,
              0, 0, 0,
              0, -1, 0);
    
    glColor3f(1, 1, 1);
    
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
    scale = 0.1f;
    rotation_y = 0.0f;
    rotation_z = 0.0f;
    vertical = 0;
    horizontal = 0;
    display_mode = DISPLAY_MODE_GOURAUD;
    lightpos[0] = 1.0f;
    lightpos[1] = -0.5f;
    lightpos[2] = 1.0f;
    lightpos[3] = 0.0f;
    matid = 0;
}
void DisplayView::end() {
    if(display_list != 0) {
        /*TODO: delete the display list */
        glDeleteLists(display_list, 1);
        display_list = 0;
    }
    DELETE(points);
    DELETE(normals);
    elements.clear();
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
    case 'd':
        display_mode = (display_mode + 1) % 4;
        if(display_list != 0) {
            glDeleteLists(display_list, 1);
            display_list = 0;
        }
        View::PostRedisplay();
        break;
    case 'D':
        display_mode = (display_mode == 0 ? 3 : (display_mode - 1));
        if(display_list != 0) {
            glDeleteLists(display_list, 1);
            display_list = 0;
        }
        View::PostRedisplay();
        break;
    case 'm':
    case 'M':
        matid = (matid + 1) % mats.size();
        std::cout << "Using material " << matid << std::endl;
        View::PostRedisplay();
        break;
    case 'h':
    case 'H':
        if(display_list != 0) {
            glDeleteLists(display_list, 1);
            display_list = 0;
        }
        std::cout << "Subdividing horizontally...";
        subdivide_horiz();
        std::cout << "done (new ring-size: " << horizontal << ")" << std::endl;
        View::PostRedisplay();
        break;
    case 'v':
    case 'V':
        if(display_list != 0) {
            glDeleteLists(display_list, 1);
            display_list = 0;
        }
        std::cout << "Subdividing vertically...";
        subdivide_vertical();
        std::cout << "done (new size: " << vertical << ")" << std::endl;
        View::PostRedisplay();
        break;
    case '+':
        scale *= 1.1;
        View::PostRedisplay();
        break;
    case '=':
        scale /= 1.1;
        View::PostRedisplay();
        break;
    case '/':
        rotation_y += 1.0f;
        View::PostRedisplay();
        break;
    case '?':
        rotation_y -= 1.0f;
        View::PostRedisplay();
        break;
    case '.':
        rotation_z += 1.0f;
        View::PostRedisplay();
        break;
    case '>':
        rotation_z -= 1.0f;
        View::PostRedisplay();
        break;
    case KEY_UP:
        _dist -= 1.0;
        std::cout << "Dist: " << _dist << std::endl;
        View::PostRedisplay();
        break;
    case KEY_DOWN:
        _dist += 1.0;
        std::cout << "Dist: " << _dist << std::endl;
        View::PostRedisplay();
        break;
    case 'r':
    case 'R':
        View::SetCurrent(*(cs354::draw));
    default:
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
void DisplayView::process(std::vector<Point3f> &controls) {
    /* Technically unsafe; should check if points.size() > size_t(-1)/3 */
    size_t num = controls.size();
    if(num <= 1) {
        View::SetCurrent(*(cs354::draw));
        return;
    }
    
    DELETE(points);
    
    /* Get the total number of points in the final model. This is an
     * overestimate, as points that lie on the axis are not actually
     * multiplied, but to keep indexing easy they need to exist in the array.
     */
    vertical = num;
    horizontal = 3;
    npoints = num * 3;
    points = new Point3f[npoints];
    /* Create new points */
    Point3f p;
    for(size_t i = 0; i < num; i++) {
        p = controls[i];
        points[i*3] = p;
        points[i*3 + 1] = Point3f(p.x * -0.5, p.y, p.x * 0.8660254037844387);
        points[i*3 + 2] = Point3f(p.x * -0.5, p.y, p.x * -0.8660254037844387);
    }
    
    update_model();
}

void DisplayView::display_points() {
    glScalef(scale, scale, scale);
    glRotatef(rotation_y, 0, 1, 0);
    glRotatef(rotation_z, 0, 0, 1);
    
    size_t npoints = this->npoints;
    glBegin(GL_POINTS); {
        for(size_t i = 0; i < npoints; ++i) {
            glVertex3f(points[i].x, points[i].y, points[i].z);
        }
    }glEnd();
}
/* This function has a LOT of overdraw -. - */
void DisplayView::display_wire() {
    glScalef(scale, scale, scale);
    glRotatef(rotation_y, 0, 1, 0);
    glRotatef(rotation_z, 0, 0, 1);
    
    size_t nelements = elements.size();
    Point3f *p1, *p2, *p3;
    glBegin(GL_LINES); {
        for(size_t i = 0; i < nelements; i += 3) {
            p1 = &points[elements[i]];
            p2 = &points[elements[i + 1]];
            p3 = &points[elements[i + 2]];
            glVertex3f(p1->x, p1->y, p1->z);
            glVertex3f(p2->x, p2->y, p2->z);
            glVertex3f(p2->x, p2->y, p2->z);
            glVertex3f(p3->x, p3->y, p3->z);
            glVertex3f(p3->x, p3->y, p3->z);
            glVertex3f(p1->x, p1->y, p1->z);
        }
    }glEnd();
}

static float _light_a[4] = {1.0, 1.0, 1.0, 1.0};
static float _light_d[4] = {1.0, 1.0, 1.0, 1.0};
static float _light_s[4] = {1.0, 1.0, 1.0, 1.0};

#define glNormal(norm) glNormal3f((norm).x, (norm).y, (norm).z)
void DisplayView::display_gouraud() {
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
    
    glLightfv(GL_LIGHT0, GL_POSITION, lightpos);
    glLightfv(GL_LIGHT0, GL_AMBIENT, _light_a);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, _light_d);
    glLightfv(GL_LIGHT0, GL_SPECULAR, _light_s);
    
    mats[matid]->use();
    Material::Red.use(FACE_BACK);
    
    glScalef(scale, scale, scale);
    glRotatef(rotation_y, 0, 1, 0);
    glRotatef(rotation_z, 0, 0, 1);
    
    if(display_list != 0) {
        glCallList(display_list);
        return;
    }
    
    display_list = glGenLists(1);
    glNewList(display_list, GL_COMPILE_AND_EXECUTE);
    
    size_t nelements = elements.size();
    Point3f *p1, *p2, *p3;
    glBegin(GL_TRIANGLES); {
        for(size_t i = 0; i < nelements; i += 3) {
            p1 = &points[elements[i]];
            p2 = &points[elements[i + 1]];
            p3 = &points[elements[i + 2]];
            glNormal(normals[elements[i]]);
            glVertex3f(p1->x, p1->y, p1->z);
            glNormal(normals[elements[i + 1]]);
            glVertex3f(p2->x, p2->y, p2->z);
            glNormal(normals[elements[i + 2]]);
            glVertex3f(p3->x, p3->y, p3->z);
        }
    }glEnd();
    glEndList();
    
    glDisable(GL_LIGHT0);
    glDisable(GL_LIGHTING);
}
void DisplayView::display_phong() {
    std::cout << "Display: Phong not implemented" << std::endl;
}

void DisplayView::create_face(uint32_t p1, uint32_t p2, uint32_t p3) {
    elements.push_back(p1);
    elements.push_back(p2);
    elements.push_back(p3);
    Vector3f normal = (points[p3] - points[p2]) * (points[p1] - points[p2]);
    normal = normal.normalize();
    normals[p1] += normal;
    normals[p2] += normal;
    normals[p3] += normal;
}
void DisplayView::update_model() {
    size_t curr = 0, prev = 0;
    bool curr_point = (points[0].x == 0.0f), prev_point = false;

    
    /* Set up the normals and elements arrays */
    DELETE(normals);
    elements.erase(elements.begin(), elements.end());
    normals = new Vector3f[npoints];
    
    /* Zero the normals */
    for(uint32_t i = 0; i < npoints; ++i) {
        normals[i] = Vector3f(0.0f, 0.0f, 0.0f);
    }
    
    for(uint32_t v = 1; v < vertical; ++v) {
        prev = curr;
        curr = prev + horizontal;
        prev_point = curr_point;
        curr_point = (points[curr].x == 0.0f);
        
        if(curr_point && prev_point) {
            /* Don't create faces between two non-ring slices */
            continue;
        }
        
        for(uint32_t h = 0; h < horizontal - 1; ++h) {
            create_face(prev + h, curr + h, curr + h + 1);
            create_face(prev + h, curr + h + 1, prev + h + 1);
        }
        create_face(prev + horizontal - 1, curr + horizontal - 1, curr);
        create_face(prev + horizontal - 1, curr, prev);
    }
    
    /* Normalize the normals (heh) */
    for(uint32_t i = 0; i < npoints; ++i) {
        normals[i] = normals[i].normalize();
    }
}

static inline Point3f _average_point(Point3f p1, Point3f p2) {
    Point3f rval = p1 * 4.0f;
    rval.x += p2.x * 4.0f;
    rval.y += p2.y * 4.0f;
    rval.z += p2.z * 4.0f;
    return rval * 0.125f;
}
static inline Point3f _average_point(Point3f p1, Point3f p2, Point3f p3) {
    Point3f rval = p2 * 6.0f;
    rval.x += p1.x + p3.x;
    rval.y += p1.y + p3.y;
    rval.z += p1.z + p3.z;
    return rval * 0.125f;
}
/* Remember to delete the display list after updating the model. */
void DisplayView::subdivide_horiz() {
    size_t nhoriz = horizontal * 2;
    size_t newsize = nhoriz * vertical;
    
    Point3f *newpoints = new Point3f[newsize];
    
    /* v is used as a slice base index */
    size_t v, pos = 0;
    for(size_t i = 0; i < vertical; ++i) {
        v = i * horizontal;
        /* Update initial point */
        newpoints[pos] = _average_point(points[v + horizontal - 1], points[v],
                                        points[v + 1]);
        newpoints[pos + 1] = _average_point(points[v], points[v + 1]);
        /* Update using the middle points */
        for(size_t h = 1; h < horizontal - 1; ++h) {
            newpoints[pos + h*2] = _average_point(points[v+h-1], points[v+h],
                                                  points[v+h+1]);
            newpoints[pos + h*2 + 1] = _average_point(points[v+h],
                                                      points[v+h+1]);
        }
        /* Update end point */
        newpoints[pos + nhoriz - 2] = _average_point(points[v+horizontal-2],
                                                     points[v+horizontal-1],
                                                     points[v]);
        newpoints[pos + nhoriz - 1] = _average_point(points[v+horizontal-1],
                                                     points[v]);
        
        pos += nhoriz;
    }
    
    DELETE(points);
    points = newpoints;
    horizontal = nhoriz;
    npoints = newsize;
    
    update_model();
}
void DisplayView::subdivide_vertical() {
    size_t nvert = vertical * 2 - 1;
    size_t newsize = horizontal * nvert;
    size_t step = horizontal * 2;
    
    Point3f *newpoints = new Point3f[newsize];
    
    /* Copy first horizontal slice, calculate second */
    for(size_t h = 0; h < horizontal; ++h) {
        newpoints[h] = points[h];
        newpoints[h + horizontal] =
            _average_point(newpoints[h], points[h + horizontal],
                           points[h + step]);
    }
    
    /* Middle slices */
    size_t oldbase, newbase;
    for(size_t v = 1; v < vertical - 1; ++v) {
        oldbase = v * horizontal;
        newbase = v * step;
        
        for(size_t h = 0; h < horizontal; ++h) {
            /* Even Rule */
            newpoints[newbase + h] =
                _average_point(points[oldbase - horizontal],
                               points[oldbase],
                               points[oldbase + horizontal]);
            /* Odd Rule */
            newpoints[newbase + h + horizontal] =
                _average_point(points[oldbase], points[oldbase + horizontal]);
        }
    }
    
    /* Copy last horizontal slice */
    oldbase = (vertical - 1) * horizontal;
    newbase = (nvert - 2) * horizontal;
    for(size_t h = 0; h < horizontal; ++h) {
        newpoints[newbase + h + horizontal] = points[oldbase + h];
    }
    DELETE(points);
    points = newpoints;
    vertical = nvert;
    npoints = newsize;
    
    update_model();
}
