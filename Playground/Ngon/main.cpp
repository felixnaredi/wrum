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
    try {
	plg::app_init();
    } catch (std::exception& e) {
	std::cerr << e.what() << "\n";
	return 1;
    }

    plg::Keyboard kb;
    plg::connect(kb);
    kb.set_key_down(GLFW_KEY_Q, [] { plg::quit(); });

    auto polygon = plg::make_polygon();
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

    while(plg::should_close() == false) {
	if(polygon.is_dirty()) {
	    glClear(GL_COLOR_BUFFER_BIT);
	    polygon.draw();
	}
    	plg::swap_buffers();
    	plg::poll_events();
	kb.update();
    }

    return 0;
}
