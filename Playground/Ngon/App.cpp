// App.cpp
//
// Author: Felix Naredi
// Date: 2018-07-18 18:30:19 +0200
//

#include <cstdio>
#include <stdexcept>
#include <sstream>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "App.hpp"

namespace plg
{
    GLFWwindow* window;
    std::function<void(int)> key_down_callback;

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
	if(action == GLFW_PRESS) { key_down_callback(key); }
    }

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

	glfwSetKeyCallback(window, key_callback);
    }

    bool should_close() noexcept { return glfwWindowShouldClose(window) == GLFW_TRUE; }
    void swap_buffers() noexcept { glfwSwapBuffers(window); }
    void wait_events() noexcept { glfwWaitEvents(); }

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
