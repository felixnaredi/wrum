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
    class Shader
    {	
	GPURef<Shader> gpu_ref_;

	auto get_iv(GLenum iv) const noexcept
	{
	    Int v;
	    glGetShaderiv(gpu_ref_, iv, &v);
	    return v;
	}	
    public:
	static void release_gpu_ref(const GPURef<Shader>& ref) noexcept
	{ glDeleteShader(ref); }
	
	static auto create_gpu_ref(GLenum type) noexcept
	{ return glCreateShader(type); }
	
	Shader(GLenum type) noexcept : gpu_ref_(type) { }

	constexpr Shader(Shader&& other) noexcept
	    : gpu_ref_(std::move(other.gpu_ref_))
	{ }

	constexpr const auto& ref() const noexcept { return gpu_ref_; }
	
	auto log() const noexcept
	{
	    auto len = get_iv(GL_INFO_LOG_LENGTH);
	    auto& ref = gpu_ref_;
	    return Log(
		len,
		[&ref, len](auto& s) {
		    glGetShaderInfoLog(ref, len, nullptr, s.data());
		});
	}
    };
}

#endif /* wrum_Shader_hpp */
