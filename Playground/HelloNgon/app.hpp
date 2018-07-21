// app.hpp
//
// Author: Felix Naredi
// Date: 2018-07-21 23:45:27 +0200
//

#ifndef app_hpp
#define app_hpp

#include <functional>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace plg
{
    extern std::function<void(int)> key_down_callback;
    
    void app_init();
    bool should_close() noexcept;
    void swap_buffers() noexcept;
    void wait_events() noexcept;
    const std::vector<char> read_file(const char* path);
    
    template <typename Lambda>
    inline
    void set_key_callback(Lambda&& l) noexcept
    { key_down_callback = l; }
}

#endif /* app_hpp */
