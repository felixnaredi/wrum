// main.cc
//
// Author: Felix Naredi
// Date: 2018-07-17 20:13:36 +0200
//

#include <iostream>
#include <array>

#include <wrum/VertexBuffer.hpp>
#include <wrum/Attrib.hpp>
#include <wrum/Shader.hpp>
#include <wrum/Program.hpp>

namespace plg
{        
    void app_init();
    bool should_close() noexcept;
    void swap_buffers() noexcept;
    void wait_events() noexcept;

    namespace Label
    {
	const auto vPosition = "vPosition";
	const auto vColor = "vColor";
    }
}

int main(int argc, char** argv)
{
    try {
	plg::app_init();
    } catch (std::exception& e) {
	std::cerr << e.what() << "\n";
	return 1;
    }

    auto vb = wrum::VertexBuffer<
	GL_STATIC_DRAW,
	wrum::Attrib<&plg::Label::vPosition>::Float2,
	wrum::Attrib<&plg::Label::vColor>::Float3>();
    
    using Vertex = decltype(vb)::Vertex;
    
    vb.encode(std::array<Vertex, 3> {
	Vertex { { -0.8, -0.8 }, { 1, 0, 0 } },
	Vertex { {  0.0,  0.8 }, { 0, 1, 0 } },
	Vertex { {  0.8, -0.8 }, { 0, 0, 1 } }
    });    
    /* init program with shaders */
    wrum::Shader<GL_VERTEX_SHADER> vxt_sh;
    vxt_sh.compile(
	"#version 130\n"
	""
	"in vec2 vPosition;"
	"in vec3 vColor;"
	""
	"out vec3 fColor;"
	""
	"void main(void)"
	"{"
	"    gl_Position = vec4(vPosition, 0, 1);"
	"    fColor = vColor;"
	"}");
    wrum::Shader<GL_FRAGMENT_SHADER> frg_sh;
    frg_sh.compile(
	"#version 130\n"
	""
	"in vec3 fColor;"
	""
	"void main(void)"
	"{"
	"    gl_FragColor = vec4(fColor, 1);"
	"}");
    wrum::Program prg;    
    prg.link(vxt_sh, frg_sh);
    
    prg.use();
    vb.locate_attribs(prg);
    vb.enable();

    glClearColor(0.1, 0.2, 0.2, 1.0);
    
    while(plg::should_close() == false) {	
	glClear(GL_COLOR_BUFFER_BIT);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	plg::swap_buffers();
	plg::wait_events();
    }
    
    return 0;
}
