
#define CS354_GENERIC_IMPL
#include "Subdiv.hpp"

#include <iostream>
#include "Geometry.hpp"
#include "Material.hpp"

using namespace cs354;

static const char *_dispmodes[] = {
    "Points",
    "Wireframe",
    "Model"
};
static const char *_shadetypes[] = {
    "Gouraud",
    "Phong (Approximation)"
};

#define dispmodestr _dispmodes[dispmode]
#define shadetypestr = _shadetypes[shadetype]

DisplayView::DisplayView() {
    flat = false;
    mats.push_back(&(Material::Bronze));
    mats.push_back(&(Material::Silver));
    mats.push_back(&(Material::Gold));
    mats.push_back(&(Material::Plastic));
    matid = 0;
    mat = Material::Bronze;
    model = NULL;
}
DisplayView::~DisplayView() {
    end();
}

void DisplayView::init() {
    BasicView::init();
    
    glPointSize(2.0f);
    glLineWidth(1.0f);

    scale = 50.0f;
    rotation_y = rotation_z = 0.0f;
    rotspeed = 1.0f;
    dispmode = DISPLAY_MODEL;
    shadetype = SHADE_GOURAUD;
    flat = true;
    matid = 0;
    light.setAmbient(Color::White);
    light.setDiffuse(Color::White);
    light.setSpecular(Color::White);
    light.setPosition(Point3f(5.0, 10.0, -52.0));
    
    mat = *(mats[0]);
    
    std::cout << "Display Mode: " << std::endl;
    std::cout << "Shading Model: " << std::endl;
}
void DisplayView::end() {
    if(model) {
        delete model;
    }
}

void DisplayView::display() {
    glShadeModel(GL_SMOOTH);
    glEnable(GL_NORMALIZE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    
    glViewport(0,0,win.dim.width, win.dim.height);
    glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, win.aspect_ratio, 1.0, 100.0);
    
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0, 0, 51.0,
              0, 0, 0,
              0, -1, 0);
    
    light.setPosition(Point3f(0, 0, 100));
    light.use(GL_LIGHT0);
    
    glScalef(scale, scale, scale);
    glRotatef(rotation_y, 0.0f, 1.0f, 0.0f);
    glRotatef(rotation_z, 0.0f, 0.0f, 1.0f);
    
    model->display(dispmode, shadetype);
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
    case 'e':
        dispmode = (dispmode == DISPLAY_MODEL ? DISPLAY_WIRES : DISPLAY_MODEL);
        std::cout << "Display mode: " << dispmodestr << std::endl;
        View::PostRedisplay();
        break;
    case 'r':
        dispmode = (dispmode == DISPLAY_MODEL ?
                    DISPLAY_POINTS : DISPLAY_MODEL);
        std::cout << "Display mode: " << dispmodestr << std::endl;
        View::PostRedisplay();
        break;
    case 'a':
        horizontal += 1;
        std::cout << "Subdividing horizontally...";
        model->subdivide(HORIZONTAL);
        std::cout << "done!" << std::endl;
        View::PostRedisplay();
        break;
    case 'b':
        vertical += 1;
        std::cout << "Subdividing vertically...";
        model->subdivide(VERTICAL);
        std::cout << "done!" << std::endl;
        View::PostRedisplay();
        break;
    case 'd':
        shadetype = (shadetype == SHADE_GOURAUD ? SHADE_PHONG : SHADE_GOURAUD);
        std::cout << "Lighting Model: " << _shadetypes[shadetype] << std::endl;
        View::PostRedisplay();
        break;
    case 's':
        if(flat) {
            flat = false;
            mat.shininess = 128;
        }else {
            flat = true;
            mat.shininess = mats[matid]->shininess;
        }
        model->setMaterial(mat);
        std::cout << "Shinieness: " << (flat ? "flat" : "shiny") << std::endl;
        View::PostRedisplay();
        break;
    case 'm':
        matid = (matid + 1) % mats.size();
        mat = *(mats[matid]);
        model->setMaterial(mat);
        View::PostRedisplay();
        break;
    case 'M':
        matid = (matid == 0 ? mats.size() - 1 : matid - 1);
        mat = *(mats[matid]);
        model->setMaterial(mat);
        View::PostRedisplay();
        break;
    case 'z':
        delete model;
        model = NULL;
        View::SetCurrent(*(cs354::draw));
        break;
    case '+':
        scale *= 1.1;
        View::PostRedisplay();
        break;
    case '-':
        scale /= 1.1;
        View::PostRedisplay();
        break;
    case 'j':
    case KEY_UP:
        rotation_z += rotspeed;
        while(rotation_z >= 360.0f) {
            rotation_z -= 360.0f;
        }
        View::PostRedisplay();
        break;
    case 'k':
    case KEY_DOWN:
        rotation_z -= rotspeed;
        while(rotation_z < 0.0f) {
            rotation_z += 360.0;
        }
        View::PostRedisplay();
        break;
    case 'h':
    case KEY_LEFT:
        rotation_y += rotspeed;
        while(rotation_y >= 360.0f) {
            rotation_y -= 360.0f;
        }
        View::PostRedisplay();
        break;
    case 'l':
    case KEY_RIGHT:
        rotation_y -= rotspeed;
        while(rotation_y < 0.0f) {
            rotation_y += 360.0f;
            break;
        }
        View::PostRedisplay();
        break;
    }
}

void DisplayView::motion(int x, int y) {
    int dx = x - mouse.pos.x, dy = y - mouse.pos.y;
    BasicView::motion(x, y);
    
    rotation_y -= dx;
    float dscale = (-dy) * 0.01;
    if(scale + dscale > 0.01) {
        scale += dscale;
    }
    View::PostRedisplay();
}

void DisplayView::make_model(const std::vector<Point3f> &control_points) {
    BasicView::init();
    
    if(control_points.size() < 3) {
        std::cout << "Cannot make model from < 3 control points" << std::endl;
        View::SetCurrent(*(cs354::draw));
        return;
    }
    
    std::vector<Point3f> modpoints;
    Point3f p;
    for(size_t i = 0; i < control_points.size(); ++i) {
        p = control_points[i];
        p.x = p.x / (float(win.dim.width));
        p.y = p.y / (float(win.dim.height));
        modpoints.push_back(p);
    }
    
    vertical = 0;
    horizontal = 0;
    model = new Model(modpoints);
}
