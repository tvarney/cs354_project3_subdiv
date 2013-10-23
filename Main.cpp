

#include <cstdlib>
#include "common.hpp"
#include "Subdiv.hpp"
#include "View.hpp"
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
    
    std::atexit(&cleanup);
    
    draw = new DrawView();
    display = new DisplayView();
    
    View::InitViewSystem(argc, argv, &win);
    View::EnterMainLoop(*draw);
    return 1;
}
