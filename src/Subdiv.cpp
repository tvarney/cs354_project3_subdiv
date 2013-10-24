
#include "Subdiv.hpp"

using namespace cs354;

BasicView *cs354::draw = NULL, *cs354::display = NULL;

DrawView::DrawView() { }
DrawView::~DrawView() { }

void DrawView::display() {
    glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void DrawView::init() { }
void DrawView::end() { }

void DrawView::keyPressed(int ch) {
    View::SetCurrent(*(cs354::display));
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
