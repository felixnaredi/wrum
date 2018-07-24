// App.cpp
//
// Author: Felix Naredi
// Date: 2018-07-18 18:30:19 +0200
//

#include <cstdio>
#include <stdexcept>
#include <sstream>
#include <vector>
#include <map>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "App.hpp"
#include "Keyboard.hpp"

namespace plg
{
    GLFWwindow* window;

    std::map<const GLFWwindow*, Keyboard*> Keyboard::window_connections_;

    void app_init()
    {
	if(glfwInit() != GLFW_TRUE) {
	    throw std::runtime_error("Failed to init GLFW");
	}
	glfwWindowHint(GLFW_SAMPLES, 4);
	window = glfwCreateWindow(377, 377, "Window", NULL, NULL);
	glfwMakeContextCurrent(window);

	auto sts = glewInit();
	if(sts != GLEW_OK) {
	    std::stringstream ss;
	    ss << "Failed to init GLEW - " << glewGetErrorString(sts);
	    throw std::runtime_error(ss.str());
	}

	glEnable(GL_MULTISAMPLE);
    }

    void quit() noexcept
    {
	glfwSetWindowShouldClose(window, GLFW_TRUE);
	// glfwTerminate();
    }

    bool should_close() noexcept { return glfwWindowShouldClose(window) == GLFW_TRUE; }
    void swap_buffers() noexcept { glfwSwapBuffers(window); }
    void wait_events() noexcept { glfwWaitEvents(); }
    void poll_events() noexcept { glfwPollEvents(); }
    void connect(Keyboard& kb) noexcept { kb.establish_window_connection(window); }

    const std::vector<char> read_file(const char* path)
    {
	auto file = std::fopen(path, "rb");
	if(file == NULL) {
	    std::stringstream ss;
	    ss << "Failed to open file '" << path << "'";
	    throw std::runtime_error(ss.str());
	}
	std::fseek(file, 0, SEEK_END);
	auto len = std::ftell(file);
	std::fseek(file, 0, SEEK_SET);

	std::vector<char> cs;
	if(len > cs.capacity()) { cs.reserve(len + 1); }
	std::fread(cs.data(), 1, len, file);
	cs[len] = '\0';

	std::fclose(file);
	return cs;
    }
}
