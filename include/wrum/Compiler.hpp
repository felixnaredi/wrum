// Compiler.hpp
//
// Author: Felix Naredi
// Date: 2018-07-19 23:55:01 +0200
//

#ifndef wrum_Compiler_hpp
#define wrum_Compiler_hpp

#include <type_traits>
#include <cstring>

#include "Shader.hpp"
#include "Program.hpp"

namespace wrum
{   
    class Compiler
    {
	template <
	    std::size_t ...Is,
	    std::size_t N = sizeof...(Is)>
	static auto src_len(
	    const std::array<const char*, N> srcs,
	    std::index_sequence<Is...>)
	noexcept
	{ return std::array<Int, N> { Int(std::strlen(srcs[Is]))... }; }
	
    public:
	template <typename Validator, std::size_t SrcN>
	void compile(
	    const Validator& validate,
	    const Shader& sh,	    
	    const std::array<const char*, SrcN>& srcs)
	{
	    auto slen = src_len(srcs, std::make_index_sequence<SrcN>());
	    glShaderSource(sh.ref(), SrcN, srcs.data(), slen.data());
	    glCompileShader(sh.ref());
	    validate(sh);
	}

	template <typename Validator>
	void compile(
	    const Validator& validate,
	    const Shader& sh,	    
	    const char* src)
	{
	    using Array = std::array<const char*, 1>;
	    compile(validate, sh, std::forward<Array>(Array {src}));
	}

	template <typename Validator, std::size_t SrcN>
	auto compile(
	    const Validator& validate,
	    GLenum sh_t,
	    const std::array<const char*, SrcN>& srcs)
	{
	    Shader sh(sh_t);
	    compile(validate, sh, srcs);
	    return std::move(sh);
	}

	template <typename Validator>
	auto compile(
	    const Validator& validate,
	    GLenum sh_t,	    
	    const char* src)
	{
	    using Array = std::array<const char*, 1>;
	    return compile(validate, sh_t, std::forward<Array>(Array {src}));
	}

	template <typename Validator, typename ...Shaders>
	auto link(const Validator& validate, const Shaders& ...shs)
	{
	    Program prg;
	    if(true) {
		auto __dmy = { (glAttachShader(prg.ref(), shs.ref()), 0)... };
	    }
	    glLinkProgram(prg.ref());
	    if(true) {
		auto __dmy = { (glDetachShader(prg.ref(), shs.ref()), 0)... };
	    }
	    validate(prg);
	    return std::move(prg);
	}
    };
}

#endif /* wrum_Compiler_hpp */
