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

    auto polygon = plg::make_polygon();

    plg::set_key_down_callback([&polygon](auto key) {
	if(key == GLFW_KEY_UP) { polygon.incr_sides(1); }
	if(key == GLFW_KEY_DOWN) { polygon.incr_sides(-1); }
    });
    std::cout << "--------------------------------\n"
	      << "Ctrl+W : Close window\n"
	      << "Up key : Increase sides of polygon\n"
	      << "Down key : Decrease sides of polygon\n";

    glClearColor(0.1, 0.2, 0.2, 1.0);

    while(plg::should_close() == false) {
	glClear(GL_COLOR_BUFFER_BIT);
	polygon.draw();
    	plg::swap_buffers();
    	plg::wait_events();
    }

    return 0;
}
