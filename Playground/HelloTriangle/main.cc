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

    auto vb = wrum::makeVertexBuffer<wrum::Memory::Static>(
	wrum::LabelField<wrum::Attrib::Float2>("vPosition"),
	wrum::LabelField<wrum::Attrib::Float3>("vColor"));	
    
    using Vertex = decltype(vb)::Vertex;
    
    vb.encode(std::array<Vertex, 3> {
	Vertex { { -0.8, -0.8 }, { 1, 0, 0 } },
	Vertex { {  0.0,  0.8 }, { 0, 1, 0 } },
	Vertex { {  0.8, -0.8 }, { 0, 0, 1 } }
    });
    
    wrum::Program prg;
    try {
    	wrum::VertexShader vxt_sh;
    	try {
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
    	}
    	catch (wrum::Exception&) {
    	    std::cerr << "Failed to compile vertex shader\n" << vxt_sh.log();
    	    return 1;
    	}
	
    	wrum::FragmentShader frg_sh;
    	try {	
    	    frg_sh.compile(
    		"#version 130\n"
    		""
    		"in vec3 fColor;"
    		""
    		"void main(void)"
    		"{"
    		"    gl_FragColor = vec4(fColor, 1);"
    		"}");
    	}
    	catch (wrum::Exception&) {
    	    std::cerr << "Failed to compile fragment shader\n" << frg_sh.log();
    	    return 1;
    	}	
    	prg.link(vxt_sh, frg_sh);
    }
    catch (wrum::Exception&) {
    	std::cerr << "Linking failed - " << prg.log() << "\n";	
    	return -1;	
    }
    
    prg.use();
    vb.locate_fields(prg);
    vb.use();

    glClearColor(0.1, 0.2, 0.2, 1.0);
    
    while(plg::should_close() == false) {	
    	glClear(GL_COLOR_BUFFER_BIT);
    	glDrawArrays(GL_TRIANGLES, 0, 3);
    	plg::swap_buffers();
    	plg::wait_events();
    }
    
    return 0;
}
