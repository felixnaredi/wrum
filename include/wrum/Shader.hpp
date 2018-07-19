// Shader.hpp
//
// Author: Felix Naredi
// Date: 2018-07-18 16:13:23 +0200
//

#ifndef wrum_Shader_hpp
#define wrum_Shader_hpp

#include <cstring>

#include "Prim.hpp"
#include "GPURef.hpp"
#include "Exception.hpp"

namespace wrum
{    
    template <Option Type>
    class Shader
    {	
	static_assert(
	    Type == GL_VERTEX_SHADER ||
	    Type == GL_FRAGMENT_SHADER);

	const GPURef gpu_ref_;

	constexpr static auto gen() { return glCreateShader(Type); }

	auto get_iv(Option iv) const noexcept
	{
	    Int v;
	    glGetShaderiv(gpu_ref_, iv, &v);
	    return v;
	}	
    public:
	constexpr Shader() noexcept : gpu_ref_(gen()) { }
	~Shader() noexcept
	{
	    glDeleteShader(gpu_ref_);
	}

	constexpr auto ref() const noexcept { return gpu_ref_; }
	constexpr auto log() const noexcept
	{
	    auto len = get_iv(GL_INFO_LOG_LENGTH);
	    auto& ref = gpu_ref_;
	    return Log(
		len,
		[&ref, len](auto& s) {
		    glGetShaderInfoLog(ref, len, nullptr, s.data());
		});
	}
	
	void compile(const char* src) const
	{
	    int len = std::strlen(src);
	    glShaderSource(gpu_ref_, 1, &src, &len);
	    glCompileShader(gpu_ref_);
	    
	    if(get_iv(GL_COMPILE_STATUS) == GL_FALSE) {
		throw Exception();
	    }
	}
    };

    using VertexShader = Shader<GL_VERTEX_SHADER>;
    using FragmentShader = Shader<GL_FRAGMENT_SHADER>;    
}

#endif /* wrum_Shader_hpp */
