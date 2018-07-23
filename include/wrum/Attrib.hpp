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
    enum class AttribVectorType
    {
	floating,
	integral,
    };

    template <typename T, Int N, bool Normalized, AttribVectorType VecT>
    struct AttribPtr { };

    template <typename T, Int N, bool Normalized>
    struct AttribPtr<T, N, Normalized, AttribVectorType::floating>
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
		TypeOption<T>::value,
		Normalized ? GL_TRUE : GL_FALSE,
		(GLsizei) stride,
		(const GLvoid*) offset);
	}
    };

    template <typename T, Int N>
    struct AttribPtr<T, N, false, AttribVectorType::integral>
    {
	static constexpr void set_loc(
	    Int loc,
	    std::size_t stride,
	    std::size_t offset)
	noexcept
	{
	    glVertexAttribIPointer(
		loc,
		N,
		TypeOption<T>::value,
		(GLsizei) stride,
		(const GLvoid*) offset);
	}
    };

    template <typename T, Int N, bool Normalized, AttribVectorType VecT>
    class BaseAttrib
    {
	std::array<T, N> data_;
    public:
	template <typename ...Args>
	constexpr BaseAttrib(Args ...args) noexcept
	    : data_ { static_cast<T>(args)... }
	{ }

	static constexpr void set_ptr(
	    Int loc,
	    std::size_t stride,
	    std::size_t offset)
	noexcept
	{ AttribPtr<T, N, Normalized, VecT>::set_loc(loc, stride, offset); }
    };

    struct Attrib
    {
	Attrib() = delete;
	Attrib(const Attrib&) = delete;

	using UByte1 = BaseAttrib<UByte, 1, false, AttribVectorType::integral>;
	using UByte2 = BaseAttrib<UByte, 2, false, AttribVectorType::integral>;
	using UByte3 = BaseAttrib<UByte, 3, false, AttribVectorType::integral>;
	using UByte4 = BaseAttrib<UByte, 4, false, AttribVectorType::integral>;
	using UByte1f = BaseAttrib<UByte, 1, false, AttribVectorType::floating>;
	using UByte2f = BaseAttrib<UByte, 2, false, AttribVectorType::floating>;
	using UByte3f = BaseAttrib<UByte, 3, false, AttribVectorType::floating>;
	using UByte4f = BaseAttrib<UByte, 4, false, AttribVectorType::floating>;
	using UByte1norm = BaseAttrib<UByte, 1, true, AttribVectorType::floating>;
	using UByte2norm = BaseAttrib<UByte, 2, true, AttribVectorType::floating>;
	using UByte3norm = BaseAttrib<UByte, 3, true, AttribVectorType::floating>;
	using UByte4norm = BaseAttrib<UByte, 4, true, AttribVectorType::floating>;

	using Byte1 = BaseAttrib<Byte, 1, false, AttribVectorType::integral>;
	using Byte2 = BaseAttrib<Byte, 2, false, AttribVectorType::integral>;
	using Byte3 = BaseAttrib<Byte, 3, false, AttribVectorType::integral>;
	using Byte4 = BaseAttrib<Byte, 4, false, AttribVectorType::integral>;
	using Byte1f = BaseAttrib<Byte, 1, false, AttribVectorType::floating>;
	using Byte2f = BaseAttrib<Byte, 2, false, AttribVectorType::floating>;
	using Byte3f = BaseAttrib<Byte, 3, false, AttribVectorType::floating>;
	using Byte4f = BaseAttrib<Byte, 4, false, AttribVectorType::floating>;
	using Byte1norm = BaseAttrib<Byte, 1, true, AttribVectorType::floating>;
	using Byte2norm = BaseAttrib<Byte, 2, true, AttribVectorType::floating>;
	using Byte3norm = BaseAttrib<Byte, 3, true, AttribVectorType::floating>;
	using Byte4norm = BaseAttrib<Byte, 4, true, AttribVectorType::floating>;

	using Int1 = BaseAttrib<Int, 1, false, AttribVectorType::integral>;
	using Int2 = BaseAttrib<Int, 2, false, AttribVectorType::integral>;
	using Int3 = BaseAttrib<Int, 3, false, AttribVectorType::integral>;
	using Int4 = BaseAttrib<Int, 4, false, AttribVectorType::integral>;
	using Int1f = BaseAttrib<Int, 1, false, AttribVectorType::floating>;
	using Int2f = BaseAttrib<Int, 2, false, AttribVectorType::floating>;
	using Int3f = BaseAttrib<Int, 3, false, AttribVectorType::floating>;
	using Int4f = BaseAttrib<Int, 4, false, AttribVectorType::floating>;
	using Int1norm = BaseAttrib<Int, 1, true, AttribVectorType::floating>;
	using Int2norm = BaseAttrib<Int, 2, true, AttribVectorType::floating>;
	using Int3norm = BaseAttrib<Int, 3, true, AttribVectorType::floating>;
	using Int4norm = BaseAttrib<Int, 4, true, AttribVectorType::floating>;

	using UInt1 = BaseAttrib<UInt, 1, false, AttribVectorType::integral>;
	using UInt2 = BaseAttrib<UInt, 2, false, AttribVectorType::integral>;
	using UInt3 = BaseAttrib<UInt, 3, false, AttribVectorType::integral>;
	using UInt4 = BaseAttrib<UInt, 4, false, AttribVectorType::integral>;
	using UInt1f = BaseAttrib<UInt, 1, false, AttribVectorType::floating>;
	using UInt2f = BaseAttrib<UInt, 2, false, AttribVectorType::floating>;
	using UInt3f = BaseAttrib<UInt, 3, false, AttribVectorType::floating>;
	using UInt4f = BaseAttrib<UInt, 4, false, AttribVectorType::floating>;
	using UInt1norm = BaseAttrib<UInt, 1, true, AttribVectorType::floating>;
	using UInt2norm = BaseAttrib<UInt, 2, true, AttribVectorType::floating>;
	using UInt3norm = BaseAttrib<UInt, 3, true, AttribVectorType::floating>;
	using UInt4norm = BaseAttrib<UInt, 4, true, AttribVectorType::floating>;

	using Float1 = BaseAttrib<Float, 1, false, AttribVectorType::floating>;
	using Float2 = BaseAttrib<Float, 2, false, AttribVectorType::floating>;
	using Float3 = BaseAttrib<Float, 3, false, AttribVectorType::floating>;
	using Float4 = BaseAttrib<Float, 4, false, AttribVectorType::floating>;
    };
}

#endif /* wrum_Attrib_hpp */
