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
    template <typename Prim>
    struct __AttribEnum { };

    template <>
    struct __AttribEnum<Byte> { const static auto value = GL_BYTE; };
    template <>
    struct __AttribEnum<UByte> { const static auto value = GL_UNSIGNED_BYTE; };
    template <>
    struct __AttribEnum<Int> { const static auto value = GL_INT; };
    template <>
    struct __AttribEnum<UInt> { const static auto value = GL_UNSIGNED_INT; };
    template <>
    struct __AttribEnum<Float> { const static auto value = GL_FLOAT; };

    template <typename Prim, Int N, bool Normalized>
    struct __AttribPtr { };

    template <Int N, bool Normalized>
    struct __AttribPtr<Float, N, Normalized>
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
		__AttribEnum<Float>::value,
		Normalized ? GL_TRUE : GL_FALSE,
		(GLsizei) stride,
		(const GLvoid*) offset);
	}
    };

    template <typename Prim, Int N, bool Normalized>
    class __Attrib
    {
	std::array<Prim, N> data_;
    public:
	template <typename ...Args>
	constexpr __Attrib(Args ...args) noexcept : data_ { Prim(args)... } { }
	
	static constexpr void set_ptr(
	    Int loc,
	    std::size_t stride,
	    std::size_t offset)
	noexcept
	{ __AttribPtr<Prim, N, Normalized>::set_loc(loc, stride, offset); }
    };

    namespace Attrib
    {
	using Float1 = __Attrib<Float, 1, false>;
	using Float2 = __Attrib<Float, 2, false>;
	using Float3 = __Attrib<Float, 3, false>;
	using Float4 = __Attrib<Float, 4, false>;
    }
}

#endif /* wrum_Attrib_hpp */
