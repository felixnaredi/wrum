// Program.hpp
//
// Author: Felix Naredi
// Date: 2018-07-18 16:34:33 +0200
//

#ifndef wrum_Program_hpp
#define wrum_Program_hpp

#include "Prim.hpp"
#include "Shader.hpp"

namespace wrum
{
    class Program
    {
	// (!) IMPLEMENT
	// -----------------------------------------------------
	// Thread safe copy that frees the program from the GPU when no
	// object has an GPU reference to it.
	//

	// (!) IMPLEMENT
	// -----------------------------------------------------
	// Error handling for the link function.
	//
	
	const UInt gpu_ref_;

	static auto gen() noexcept { return glCreateProgram(); }
    public:
	Program() : gpu_ref_(gen()) { }

	constexpr auto ref() const noexcept { return gpu_ref_; }

	void use() const noexcept { glUseProgram(gpu_ref_); }
	void unuse() const noexcept { glUseProgram(0); }

	template<GLenum ...Type>
	void link(Shader<Type> ...shs) const
	{
	    auto dummy0 = { (glAttachShader(gpu_ref_, shs.ref()), 0)... };
	    glLinkProgram(gpu_ref_);	    
	    /* validate */
	    auto dummy1 = { (glDetachShader(gpu_ref_, shs.ref()), 0)... };
	}
    };
}

#endif /* wrum_Program_hpp */
