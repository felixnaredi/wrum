// main.cc
//
// Author: Felix Naredi
// Date: 2018-07-17 20:13:36 +0200
//

#include <iostream>
#include <array>

#include <wrum/Attrib.hpp>
#include <wrum/Field.hpp>
#include <wrum/VertexBuffer.hpp>
#include <wrum/Compiler.hpp>
#include <wrum/Shader.hpp>
#include <wrum/Program.hpp>

namespace plg
{
    void app_init();
    bool should_close() noexcept;
    void swap_buffers() noexcept;
    void wait_events() noexcept;
}

int main(int argc, char** argv)
{
    try {
	plg::app_init();
    } catch (std::exception& e) {
	std::cerr << e.what() << "\n";
	return 1;
    }

    auto vb = wrum::make_vertex_buffer<wrum::BufferMemory::Static>(
	wrum::LabelField<wrum::Attrib::Float2>("vPosition"),
	wrum::LabelField<wrum::Attrib::UByte3norm>("vColor"));

    using Vertex = decltype(vb)::Vertex;

    vb.encode(std::array<Vertex, 3> {
    	Vertex { { -0.8, -0.8 }, { 255, 128,   0 } },
    	Vertex { {    0,  0.8 }, {   0, 255, 128 } },
    	Vertex { {  0.8, -0.8 }, { 128,   0, 255 } }
    });

    wrum::Compiler compiler;

    int stage = 0;
    const auto put_log = [&stage](const auto& e) {
	std::cout << "STAGE: " << stage++ << "\n"
	          << e.log() << "\n";
    };

    auto prg = compiler.link(
	put_log,
	std::forward<wrum::Shader>(
	    compiler.compile(
		put_log,
		GL_VERTEX_SHADER,
		std::forward<const char*>(
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
		    "}"))),
	std::forward<wrum::Shader>(
	    compiler.compile(
		put_log,
		GL_FRAGMENT_SHADER,
		std::forward<const char*>(
		    "#version 130\n"
		    ""
		    "in vec3 fColor;"
		    ""
		    "void main(void)"
		    "{"
		    "    gl_FragColor = vec4(fColor, 1);"
		    "}"))));

    prg.use();
    vb.locate_fields(prg);
    vb.bind();

    glClearColor(0.1, 0.2, 0.2, 1.0);

    while(plg::should_close() == false) {
    	glClear(GL_COLOR_BUFFER_BIT);
    	glDrawArrays(GL_TRIANGLES, 0, 3);
    	plg::swap_buffers();
    	plg::wait_events();
    }

    return 0;
}
