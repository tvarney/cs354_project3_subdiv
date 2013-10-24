

#include <cstdlib>
#include "common.hpp"
#include "Subdiv.hpp"
#include "View.hpp"
#include "Matrix.hpp"
#include "Window.hpp"

using namespace cs354;

void cleanup() {
    if(draw) {
        delete draw;
        draw = NULL;
    }
    if(display) {
        delete display;
        display = NULL;
    }
}

int main(int argc, char **argv) {
    Window win("Subdiv - Troy Varney", 400, 400, 10, 10,
               Window::RGB | Window::DOUBLE | Window::DEPTH);
    
    Matrix<double> m1 = Matrix<double>::Identity();
    Matrix<double> m2 = Matrix<double>::Identity().translate(3,4,5);
    Matrix<double> m3 = m2 * m1;
    
    std::atexit(&cleanup);

    View::InitViewSystem(argc, argv, &win);
    draw = new DrawView();
    display = new DisplayView();
    
    View::EnterMainLoop(*draw);
    return 1;
}
