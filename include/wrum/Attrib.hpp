// Attrib.hpp
//
// Author: Felix Naredi
// Date: 2018-07-18 00:33:45 +0200
//

#ifndef wrum_Attrib_hpp
#define wrum_Attrib_hpp

#include <array>

#include "Prim.hpp"

namespace wrum
{
    template <typename Prim, Int N, bool Normalized>
    struct AttribPtr { };

    template <Int N, bool Normalized>
    struct AttribPtr<Float, N, Normalized>
    {
	static constexpr void set_loc(
	    Int loc,
	    std::size_t stride,
	    std::size_t offset)
	noexcept
	{
	    glVertexAttribPointer(
		loc,
		N,		
		TypeOption<Float>::value,
		Normalized ? GL_TRUE : GL_FALSE,
		(GLsizei) stride,
		(const GLvoid*) offset);
	}
    };

    template <typename Prim, Int N, bool Normalized>
    class BaseAttrib
    {
	std::array<Prim, N> data_;
    public:
	template <typename ...Args>
	constexpr BaseAttrib(Args ...args) noexcept : data_ { Prim(args)... } { }
	
	static constexpr void set_ptr(
	    Int loc,
	    std::size_t stride,
	    std::size_t offset)
	noexcept
	{ AttribPtr<Prim, N, Normalized>::set_loc(loc, stride, offset); }
    };

    struct Attrib
    {
	Attrib() = delete;
	Attrib(const Attrib&) = delete;
	
	using Float1 = BaseAttrib<Float, 1, false>;
	using Float2 = BaseAttrib<Float, 2, false>;
	using Float3 = BaseAttrib<Float, 3, false>;
	using Float4 = BaseAttrib<Float, 4, false>;	
    };
}

#endif /* wrum_Attrib_hpp */
