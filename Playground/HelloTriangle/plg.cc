// plg.cc
//
// Author: Felix Naredi
// Date: 2018-07-18 18:30:19 +0200
//

#include <stdexcept>
#include <sstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace plg
{
    GLFWwindow* window;    

    void key_callback(
	GLFWwindow* window,
	int key,
	int scancode,
	int action,
	int mods)
    {
	if((mods & GLFW_MOD_CONTROL) && key == GLFW_KEY_W) {
	    glfwSetWindowShouldClose(window, GLFW_TRUE);	    
	}
    }
    
    void app_init()
    {
	if(glfwInit() != GLFW_TRUE) {
	    throw std::runtime_error("Failed to init GLFW");
	}
	window = glfwCreateWindow(377, 377, "Window", NULL, NULL);
	glfwMakeContextCurrent(window);

	auto sts = glewInit();
	if(sts != GLEW_OK) {
	    std::stringstream ss;
	    ss << "Failed to init GLEW - " << glewGetErrorString(sts);
	    throw std::runtime_error(ss.str());
	}

	glfwSetKeyCallback(window, key_callback);
    }

    bool should_close() noexcept { return glfwWindowShouldClose(window) == GLFW_TRUE; }
    void swap_buffers() noexcept { glfwSwapBuffers(window); }
    void wait_events() noexcept { glfwWaitEvents(); }
}
