
#include "Model.hpp"

#include <iostream>
#include "common.hpp"
#include "glMacros.hpp"
#include "Geometry.hpp"
#include "Material.hpp"

using namespace cs354;

Model::Model(const std::vector<Point3f> & control_points) :
    display_list(0), width(3), height(control_points.size()),
    size(width * height), points(NULL), normals(NULL), nfaces(NULL),
    material(&Material::Bronze), lastmode(DISPLAY_POINTS),
    lasttype(SHADE_PHONG), _depth(1)
{
    if(height <= 1) {
        throw "Too few control points";
    }
    
    points = new Point3f[size];
    Point3f p;
    size_t pos = 0;
    for(size_t i = 0; i < height; i++) {
        p = control_points[i];
        points[pos] = p;
        points[pos + 1] = Point3f(p.x * -0.5, p.y, p.x * 0.8660254037844387);
        points[pos + 2] = Point3f(p.x * -0.5, p.y, p.x * -0.8660254037844387);
        
        std::cout << "Point " << pos << ": " << points[pos] << std::endl;
        std::cout << "Point " << pos + 1 << ": " << points[pos+1] << std::endl;
        std::cout << "Point " << pos + 2 << ": " << points[pos+2] << std::endl;
        pos += 3;
    }
    update();
}

Model::~Model() {
    if(points != NULL) {
        delete[] points;
    }
    if(normals != NULL) {
        delete[] normals;
    }
}

void Model::display(DisplayMode mode, ShadeType type) {
    /* Check for change to the current drawing routine. If it has changed,
     * get rid of the display list.
     */
    if(mode != lastmode || type != lasttype) {
        lastmode = mode;
        lasttype = type;
        if(display_list != 0) {
            glDeleteLists(display_list, 1);
            display_list = 0;
        }
    }
    
    material->use();
    
    /* Use the display list if possible, otherwise bail */
    if(display_list != 0) {
        glCallList(display_list);
    }else {
        display_list = glGenLists(1);
        glNewList(display_list, GL_COMPILE_AND_EXECUTE);
        
        uint32_t e1, e2, e3;
        Point3f p1, p2, p3;
        Vector3f n1, n2, n3;
        uint32_t d = (type == SHADE_GOURAUD ? 0 : _depth);
        GLenum type = (mode == DISPLAY_POINTS ? GL_POINTS :
                       (mode == DISPLAY_WIRES ? GL_LINES : GL_TRIANGLES));
        glBegin(type);
        for(size_t i = 0; i < elements.size(); i += 3) {
            e1 = elements[i]; e2 = elements[i+1]; e3 = elements[i+2];
            p1 = points[e1];  p2 = points[e2];    p3 = points[e3];
            n1 = normals[e1]; n2 = normals[e2];   n3 = normals[e3];
            r_subdiv_face(d, p1, p2, p3, n1, n2, n3);
        }
        glEnd();
        glEndList();
    }
}

void Model::subdivide(Direction dir) {
    switch(dir) {
    case HORIZONTAL:
        subdiv_h();
        break;
    case VERTICAL:
        subdiv_v();
        break;
    }
    if(display_list != 0) {
        glDeleteLists(display_list, 1);
        display_list = 0;
    }
}

void Model::setMaterial(const Material &material) {
    this->material = &(material);
    if(display_list != 0) {
        glDeleteLists(display_list, 1);
        display_list = 0;
    }
}
int Model::depth() const {
    return _depth;
}
void Model::depth(int newdepth) {
    _depth = (newdepth >= 0 ? newdepth : 0);
}

void Model::create_face(uint32_t p1, uint32_t p2, uint32_t p3) {
    elements.push_back(p1);
    elements.push_back(p2);
    elements.push_back(p3);
    Vector3f normal = (points[p1] - points[p2]) * (points[p3] - points[p2]);
    normal = normal.normalize();
    nfaces[p1] += 1;
    nfaces[p2] += 1;
    nfaces[p3] += 1;
    normals[p1] += normal;
    normals[p2] += normal;
    normals[p3] += normal;
}

void Model::update() {
    if(normals != NULL) {
        delete[] normals;
        normals = NULL;
    }
    if(nfaces != NULL) {
        delete[] nfaces;
        nfaces = NULL;
    }
    elements.erase(elements.begin(), elements.end());
    
    normals = new Vector3f[size];
    nfaces = new uint32_t[size];
    
    for(size_t i = 0; i < size; ++i) {
        normals[i] = Vector3f(0,0,0);
        nfaces[i] = 0;
    }
    
    size_t curr = 0, prev = 0;
    bool curr_point = (points[0].x == 0.0f), prev_point = false;
    for(uint32_t v = 1; v < height; ++v) {
        prev = curr;
        curr = prev + width;
        prev_point = curr_point;
        curr_point = (points[curr].x == 0.0f);
        
        if(curr_point == true) {
            if(prev_point == true) {
                /* point to point (0 triangles) */
                continue;
            }
            
            /* ring to point (width triangles) */
            for(uint32_t h = 0; h < width - 1; ++h) {
                create_face(prev + h, curr, prev + h + 1);
            }
            create_face(prev + width - 1, curr, prev);
        }else if(prev_point == true) {
            /* point to ring (width triangles) */
            for(uint32_t h = 0; h < width - 1; ++h) {
                create_face(prev, curr + h + 1, curr + h);
            }
            create_face(prev, curr + width - 1, curr);
        }else {
            /* Two rings (width * 2 triangles) */
            for(uint32_t h = 0; h < width - 1; ++h) {
                create_face(prev + h, curr + h, curr + h + 1);
                create_face(prev + h, curr + h + 1, prev + h + 1);
            }
            create_face(prev + width - 1, curr + width - 1, curr);
            create_face(prev + width - 1, curr, prev);
        }
    }
    
    /* Normalize the normal vectors after finding the average */
    for(size_t i = 0; i < size; ++i) {
        if(nfaces[i] > 0) {
            normals[i] = (normals[i] / float(nfaces[i])).normalize();
        }
    }
}

static inline
Point3f _oddrule(const Point3f p1, const Point3f p2) {
    return Point3f(p1.x + p2.x, p1.y + p2.y, p1.z + p2.z) * 0.5f;
}
static inline
Point3f _evenrule(const Point3f p1, const Point3f p2, const Point3f p3) {
    return Point3f(p1.x + (p2.x * 6.0f) + p3.x,
                   p1.y + (p2.y * 6.0f) + p3.y,
                   p1.z + (p2.z * 6.0f) + p3.z) * 0.125f;
}
void Model::subdiv_h() {
    size_t nwidth = width * 2;
    size_t nsize = nwidth * height;
    Point3f *npoints = new Point3f[nsize];
    size_t v, pos = 0;
    for(size_t i = 0; i < height; ++i, pos += nwidth) {
        v = i * width;
        /* Initial Pair */
        npoints[pos] = _evenrule(points[v+width-1], points[v], points[v+1]);
        npoints[pos+1] = _oddrule(points[v], points[v+1]);
        /* Middle pairs */
        for(size_t h = 1; h < width - 1; ++h) {
            npoints[pos+h*2] = _evenrule(points[v+h-1], points[v+h],
                                         points[v+h+1]);
            npoints[pos+h*2+1] = _oddrule(points[v+h], points[v+h+1]);
        }
        /* End pair */
        npoints[pos+nwidth-2] = _evenrule(points[v+width-2], points[v+width-1],
                                          points[v]);
        npoints[pos+nwidth-1] = _oddrule(points[v+width-1], points[v]);
    }
    
    delete[] points;
    points = npoints;
    width = nwidth;
    size = nsize;
    
    update();
}

#define pos(h, v) ((v)*width + (h))
void Model::subdiv_v() {
    size_t nheight = height * 2 - 1;
    size_t nsize = width * nheight;
    Point3f *npoints = new Point3f[nsize];
    
    for(size_t h = 0; h < width; ++h) {
        /* Initial point of this vertical slice stays the same */
        npoints[h] = points[h];
        npoints[pos(h, 1)] = _oddrule(points[h], points[pos(h, 1)]);
        
        /* Middle points */
        for(size_t v = 1; v < height - 1; ++v) {
            npoints[pos(h, v*2)] =
                _evenrule(points[pos(h, v-1)], points[pos(h, v)],
                          points[pos(h, v+1)]);
            npoints[pos(h, v*2+1)] =
                _oddrule(points[pos(h, v)], points[pos(h, v +1)]);
        }
        /* Last point stays the same */
        npoints[pos(h, nheight-1)] = points[pos(h, height-1)];
    }
    
    delete[] points;
    points = npoints;
    height = nheight;
    size = nsize;
    
    update();
}

void Model::r_subdiv_face(int n, Point3f p1, Point3f p2, Point3f p3,
                          Vector3f n1, Vector3f n2, Vector3f n3)
{
    /* Draw the face if n <= 0 */
    if(n <= 0) {
        switch(lastmode) {
        case DISPLAY_WIRES:
            glNormal(n1); glVertex(p1);
            glNormal(n2); glVertex(p2);
            glNormal(n2); glVertex(p2);
            glNormal(n3); glVertex(p3);
            glNormal(n3); glVertex(p3);
            glNormal(n1); glVertex(p1);
            break;
        case DISPLAY_POINTS:
        case DISPLAY_MODEL:
            glNormal(n1); glVertex(p1);
            glNormal(n2); glVertex(p2);
            glNormal(n3); glVertex(p3);
            break;
        }
    }else {
        Point3f pm1 = lerp(p1, p2), pm2 = lerp(p1, p3), pm3 = lerp(p2, p3);
        Vector3f nm1 = lerp(n1, n2), nm2 = lerp(n1, n3), nm3 = lerp(n2, n3);
        n -= 1;
        r_subdiv_face(n, p1, pm1, pm2, n1, nm1, nm2);
        r_subdiv_face(n, pm1, pm3, pm2, nm1, nm3, nm2);
        r_subdiv_face(n, pm1, p2, pm3, nm1, n2, nm3);
        r_subdiv_face(n, pm2, pm3, p3, nm2, nm3, n3);
    }
}
