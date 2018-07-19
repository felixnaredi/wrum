// Program.hpp
//
// Author: Felix Naredi
// Date: 2018-07-18 16:34:33 +0200
//

#ifndef wrum_Program_hpp
#define wrum_Program_hpp

#include <atomic>

#include "Prim.hpp"
#include "GPURef.hpp"
#include "Shader.hpp"
#include "Exception.hpp"

namespace wrum
{
    class Program
    {	
	const GPURef gpu_ref_;
	std::atomic_bool used_;

	static auto gen() noexcept { return glCreateProgram(); }

	auto get_iv(GLenum iv) const noexcept
	{
	    Int r;
	    glGetProgramiv(gpu_ref_, iv, &r);
	    return r;
	}
    public:
	Program() noexcept : gpu_ref_(gen()), used_(false) { }
	~Program() noexcept { glDeleteProgram(gpu_ref_); }

	constexpr auto ref() const noexcept { return gpu_ref_; }
	auto is_used() const noexcept { return used_.load(); }	
	
	void use() noexcept
	{
	    if(used_.load()) { return; }
	    glUseProgram(gpu_ref_);
	    used_.store(true);
	}
	
	void unuse() noexcept
	{
	    if(used_.load() == false) { return; }
	    glUseProgram(0);
	    used_.store(false);
	}

	auto log() const noexcept
	{
	    auto len = get_iv(GL_INFO_LOG_LENGTH);
	    const auto& ref = gpu_ref_;
	    return Log(
		len,
		[&ref, len](auto& s) {
		    glGetProgramInfoLog(ref, len, nullptr, s.data());
		});
	}

	template<GLenum ...Type>
	void link(const Shader<Type>& ...shs) const
	{
	    auto __dmy0 = { (glAttachShader(gpu_ref_, shs.ref()), 0)... };
	    glLinkProgram(gpu_ref_);	    	    
	    auto __dmy1 = { (glDetachShader(gpu_ref_, shs.ref()), 0)... };

	    if(get_iv(GL_LINK_STATUS) == GL_FALSE) { throw Exception(); }
	}
    };
}

#endif /* wrum_Program_hpp */
