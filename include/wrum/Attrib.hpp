// Attrib.hpp
//
// Author: Felix Naredi
// Date: 2018-07-18 00:33:45 +0200
//

#ifndef wrum_Attrib_hpp
#define wrum_Attrib_hpp

#include "Prim.hpp"
#include "Program.hpp"

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

    template <const auto* Label>
    class Attrib
    {
	template <typename Prim, Int N>
	class AttribI
	{	
	    std::array<Prim, N> data_;
	public:
	    constexpr static auto locate(const Program& prg) noexcept
	    {
		return glGetAttribLocation(prg.ref(), *Label);
	    }
	    
	    template <typename ...Args>
	    constexpr AttribI(Args ...args) : data_ {Prim(args)...} { }

	    static constexpr void setp(
		Int location,
		std::size_t stride,
		std::size_t offset)
	    {
		glVertexAttribIPointer(
		    location,
		    N,
		    __AttribEnum<Prim>::value,
		    (GLsizei) stride,
		    (const GLvoid*) offset);
	    }
	};
	
	template <typename Prim, Int N, bool Normalized>
	class AttribF
	{	
	    std::array<Prim, N> data_;
	public:
	    constexpr static auto locate(const Program& prg) noexcept
	    {
		return glGetAttribLocation(prg.ref(), *Label);
	    }
	    
	    template <typename ...Args>
	    constexpr AttribF(Args ...args) : data_ {Prim(args)...} { }

	    constexpr static void setp(
		Int location,
		std::size_t stride,
		std::size_t offset)
	    {
		glVertexAttribPointer(
		    location,
		    N,
		    __AttribEnum<Prim>::value,
		    Normalized ? GL_TRUE : GL_FALSE,
		    (GLsizei) stride,
		    (const GLvoid*) offset);
	    }
	};
    public:	
	using Byte1 = AttribI<Byte, 1>;
	using Byte2 = AttribI<Byte, 2>;
	using Byte3 = AttribI<Byte, 3>;
	using Byte4 = AttribI<Byte, 4>;
	using UByte1 = AttribI<UByte, 1>;
	using UByte2 = AttribI<UByte, 2>;
	using UByte3 = AttribI<UByte, 3>;
	using UByte4 = AttribI<UByte, 4>;
	using Int1 = AttribI<Int, 1>;
	using Int2 = AttribI<Int, 2>;
	using Int3 = AttribI<Int, 3>;
	using Int4 = AttribI<Int, 4>;
	using UInt1 = AttribI<UInt, 1>;
	using UInt2 = AttribI<UInt, 2>;
	using UInt3 = AttribI<UInt, 3>;
	using UInt4 = AttribI<UInt, 4>;	
	using Float1 = AttribF<Float, 1, false>;
	using Float1n = AttribF<Float, 1, true>;
	using Float2 = AttribF<Float, 2, false>;
	using Float2n = AttribF<Float, 2, true>;
	using Float3 = AttribF<Float, 3, false>;
	using Float3n = AttribF<Float, 3, true>;
	using Float4 = AttribF<Float, 4, false>;
	using Float4n = AttribF<Float, 4, true>;
    };
}

#endif /* wrum_Attrib_hpp */
