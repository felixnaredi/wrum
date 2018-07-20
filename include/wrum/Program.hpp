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
	GPURef<Program> gpu_ref_;
	std::atomic_bool used_;	
    public:
	static auto create_gpu_ref() noexcept
	{ return glCreateProgram(); }
	
	static void release_gpu_ref(const GPURef<Program>& ref) noexcept
	{ glDeleteProgram(ref); }
	
	Program() noexcept : used_(false) { }
	
	Program(Program&& other) noexcept
	    : gpu_ref_(std::move(other.gpu_ref_)),
	      used_(other.used_.load())
	{ }

	constexpr const auto& ref() const noexcept { return gpu_ref_; }
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

	auto get_iv(GLenum iv) const noexcept
	{
	    Int r;
	    glGetProgramiv(gpu_ref_, iv, &r);
	    return r;
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
    };
}

#endif /* wrum_Program_hpp */
