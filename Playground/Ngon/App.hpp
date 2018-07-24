// App.hpp
//
// Author: Felix Naredi
// Date: 2018-07-21 23:45:27 +0200
//

#ifndef plg_App_hpp
#define plg_App_hpp

#include <optional>
#include <vector>
#include <array>
#include <functional>
#include <sstream>
#include <algorithm>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Keyboard.hpp"

namespace plg
{
    class Window
    {
	std::optional<GLFWwindow*> win_;
	bool open_;

	void swap_buffers() noexcept { glfwSwapBuffers(*win_); }
    public:
	std::function<void()> buffers_will_swap;

	Window(int w, int h)
	    : win_(glfwCreateWindow(w, h, "Window", NULL, NULL)),
	      open_(false)
	{ }

	enum class Exception
	{
	    null_window,
	};

	constexpr auto& window_ref() const
	{
	    if(win_ == std::nullopt) { throw Exception::null_window; }
	    return *win_;
	}

	void make_context_current() const
	{ glfwMakeContextCurrent(window_ref()); }

	constexpr void disable() noexcept
	{
	    win_ = std::nullopt;
	    open_ = false;
	}

	constexpr bool is_disabled() const noexcept { return win_ == std::nullopt; }
	constexpr bool is_open() const noexcept { return open_; }

	void update()
	{
	    buffers_will_swap();
	    swap_buffers();
	}

	void close()
	{
	    glfwSetWindowShouldClose(window_ref(), true);
	    open_ = false;
	}

	template <class T>
	void connect(T& e)
	{ e.establish_window_connection(window_ref()); }
    };

    template <std::size_t WindowLimit>
    class App
    {
	static void poll_events() { glfwPollEvents(); }

	std::array<std::optional<Window>, WindowLimit> windows_;
	bool require_init_;
	bool did_quit_;
    public:
	enum class Exception
	{
	    window_limit_reached,
	    app_did_quit,
	};
	App() : require_init_(true), did_quit_(false) { }

	auto& make_window(int w, int h)
	{
	    if(did_quit_) { throw Exception::app_did_quit; }

	    if(require_init_) {
		if(glfwInit() != GLFW_TRUE) {
		    throw std::runtime_error("Failed to init GLFW");
		}
	    }
	    auto it = std::find_if(
		windows_.begin(),
		windows_.end(),
		[](auto& win) { return win.has_value() == false; });
	    if(it == windows_.end()) { throw Exception::window_limit_reached; }

	    *it = Window(w, h);

	    if(require_init_) {
		(*it)->make_context_current();
		auto sts = glewInit();
		if(sts != GLEW_OK) {
		    std::stringstream ss;
		    ss << "Failed to init GLEW - " << glewGetErrorString(sts);
		    throw std::runtime_error(ss.str());
		}
		require_init_ = false;
	    }
	    return **it;
	}

	bool has_open_windows() const noexcept
	{
	    return std::find_if(
		windows_.begin(),
		windows_.end(),
		[](auto& win) {
		    if(win == std::nullopt) { return false; }
		    return win->is_open() == false;
		}) != windows_.end();
	}

	void run()
	{
	    while(has_open_windows()) {
		for(auto& win : windows_) {
		    if(win.has_value()) { win->update(); }
		}
		poll_events();
	    }
	    if(did_quit_) {
		glfwTerminate();
		require_init_ = true;
	    }
	}

	constexpr void quit()
	{
	    for(auto& win : windows_) {
		if(win.has_value()) {
		    win->disable();
		    win = std::nullopt;
		}
	    }
	    did_quit_ = true;
	}
    };

    std::vector<char> read_file(const char* path)
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

#endif /* plg_App_hpp */
