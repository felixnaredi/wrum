// Keyboard.hpp
//
// Author: Felix Naredi
// Date: 2018-07-23 23:09:09 +0200
//

#ifndef plg_Keyboard_hpp
#define plg_Keyboard_hpp

#include <map>
#include <list>
#include <functional>
#include <chrono>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace plg
{
    struct Keyboard
    {
	using KeyCode = decltype(GLFW_KEY_A);
    private:
	using KeyMap = std::map<KeyCode, std::function<void()>>;
	using Clock = std::chrono::steady_clock;
	using Millisec = std::chrono::milliseconds;

	static std::map<const GLFWwindow*, Keyboard*> window_connections_;

	static void call(const KeyMap& kbm, KeyCode k)
	{
	    auto callback = kbm.find(k);
	    if(callback == kbm.end()) { return; }
	    callback->second();
	}

	KeyMap key_down_map_;
	KeyMap key_up_map_;
	KeyMap key_hold_map_;
	std::list<KeyCode> hold_keys_;
	bool hold_on_;
	bool hold_active_;
	Clock::time_point hold_check_time_;
	Millisec hold_dur_;

	void press(KeyCode key)
	{
	    call(key_down_map_, key);
	    hold_on_ = hold_keys_.empty();
	    if(hold_on_) { hold_check_time_ = Clock::now(); }
	    hold_keys_.push_back(key);
	}

	void release(KeyCode key)
	{
	    call(key_up_map_, key);
	    hold_keys_.remove(key);
	    hold_on_ = false;
	    hold_active_ = false;
	    hold_dur_ = Millisec(0);
	}

	static void window_key_callback(
	    GLFWwindow* win,
	    int key,
	    int scancode,
	    int action,
	    int mods)
	{
	    auto kb = window_connections_[win];
	    if(kb == nullptr) { return; }

	    switch (action) {
	    case GLFW_PRESS:
		kb->press(key);
		break;
	    case GLFW_RELEASE:
		kb->release(key);
		break;
	    }
	}
    public:
	Keyboard() noexcept
	    : hold_on_(false),
	      hold_active_(false),
	      hold_dur_(0)
	{ }

	template <typename Lambda>
	void set_key_down(KeyCode k, const Lambda l)
	{ key_down_map_[k] = l; }

	template <typename Lambda>
	void set_key_up(KeyCode k, const Lambda l)
	{ key_up_map_[k] = l; }

	template <typename Lambda>
	void set_key_hold(KeyCode k, const Lambda l)
	{ key_hold_map_[k] = l; }

	void establish_window_connection(GLFWwindow* win)
	{
	    glfwSetKeyCallback(win, window_key_callback);
	    window_connections_[win] = this;
	}

	void update()
	{
	    if(hold_on_ == false) { return; }

	    auto now = Clock::now();
	    hold_dur_ += std::chrono::duration_cast<Millisec>(now - hold_check_time_);
	    hold_check_time_ = now;

	    if(hold_active_ == false) {
		const Millisec d(280);
		if(hold_dur_ > d) {
		    hold_active_ = true;
		    hold_dur_ %= d;
		}
	    } else {
		const Millisec d(70);
		if(hold_dur_ > d) {
		    call(key_hold_map_, hold_keys_.front());
		    hold_dur_ %= d;
		}
	    }
	}
    };

    std::map<const GLFWwindow*, Keyboard*> Keyboard::window_connections_;
}

#endif /* plg_Keyboard_hpp */
