// Shader.hpp
//
// Author: Felix Naredi
// Date: 2018-07-18 16:13:23 +0200
//

#ifndef wrum_Shader_hpp
#define wrum_Shader_hpp

#include <cstring>

#include "Prim.hpp"

namespace wrum
{
    template <GLenum Type>
    class Shader
    {
	// (!) IMPLEMENT
	// -----------------------------------------------------
	// Thread safe copy that frees the shader from the GPU when no
	// object has an GPU reference to it.
	//

	// (!) IMPLEMENT
	// -----------------------------------------------------
	// Error handling for the compile opreration.
	//
	
	static_assert(
	    Type == GL_VERTEX_SHADER ||
	    Type == GL_FRAGMENT_SHADER);

	const UInt gpu_ref_;

	constexpr static auto gen() { return glCreateShader(Type); }
    public:
	constexpr Shader() noexcept : gpu_ref_(gen()) { }

	void compile(const char* src) const
	{
	    int len = std::strlen(src);
	    glShaderSource(gpu_ref_, 1, &src, &len);
	    glCompileShader(gpu_ref_);
	    /* validate */
	}

	constexpr auto ref() const noexcept { return gpu_ref_; } 
    };
}

#endif /* wrum_Shader_hpp */
