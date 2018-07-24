// App.hpp
//
// Author: Felix Naredi
// Date: 2018-07-21 23:45:27 +0200
//

#ifndef plg_App_hpp
#define plg_App_hpp

#include <functional>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Keyboard.hpp"

namespace plg
{
    void app_init();
    void quit() noexcept;
    bool should_close() noexcept;
    void swap_buffers() noexcept;
    void wait_events() noexcept;
    void poll_events() noexcept;
    void connect(Keyboard& kb) noexcept;
    const std::vector<char> read_file(const char* path);
}

#endif /* plg_App_hpp */
