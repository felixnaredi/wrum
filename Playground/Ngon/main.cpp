// main.cpp
//
// Author: Felix Naredi
// Date: 2018-07-21 17:33:52 +0200
//

#include <iostream>
#include <stdexcept>

#include "App.hpp"
#include "Polygon.hpp"

int main(int argc, char** argv)
{
    plg::App<1> app;
    auto& win = app.make_window(377, 377);

    plg::Keyboard kb;
    win.connect(kb);
    kb.set_key_down(GLFW_KEY_Q, [&app] { app.quit(); });

    auto polygon = plg::make_polygon();

    win.buffers_will_swap = [&polygon, &kb] {
    	kb.update();
    	if(polygon.is_dirty() == false) { return; }
    	glClear(GL_COLOR_BUFFER_BIT);
    	polygon.draw();
    };

    auto incr_polygon_sides = [&polygon](int n) {
    	return [&polygon, n] {
    	    polygon.incr_sides(n);
    	    std::cout << "Sides: " << polygon.sides() << '\n';
    	};
    };

    kb.set_key_down(GLFW_KEY_UP, incr_polygon_sides(1));
    kb.set_key_hold(GLFW_KEY_UP, incr_polygon_sides(1));
    kb.set_key_down(GLFW_KEY_DOWN, incr_polygon_sides(-1));
    kb.set_key_hold(GLFW_KEY_DOWN, incr_polygon_sides(-1));

    std::cout << "--------------------------------\n"
    	      << "Q    : Quit program\n"
    	      << "Up   : Increase sides of polygon\n"
    	      << "Down : Decrease sides of polygon\n"
    	      << '\n';

    glClearColor(0.1, 0.2, 0.2, 1.0);
    app.run();

    return 0;
}
