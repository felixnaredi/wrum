// Prim.hpp
//
// Author: Felix Naredi
// Date: 2018-07-17 19:45:11 +0200
//

#ifndef wrum_Prim_hpp
#define wrum_Prim_hpp

#include <GL/glew.h>

namespace wrum
{
    using Byte = GLbyte;
    using UByte = GLubyte;
    using Int = GLint;
    using UInt = GLuint;
    using Float = GLfloat;
    using Option = GLenum;

    enum class BufferMemory
    {
	Stream = GL_STREAM_DRAW,
	Dynamic = GL_DYNAMIC_DRAW,
	Static = GL_STATIC_DRAW,
    };

    template <typename T>
    struct TypeOption { };

    template <>
    struct TypeOption<Byte> { const static auto value = GL_BYTE; };
    template <>
    struct TypeOption<UByte> { const static auto value = GL_UNSIGNED_BYTE; };
    template <>
    struct TypeOption<Int> { const static auto value = GL_INT; };
    template <>
    struct TypeOption<UInt> { const static auto value = GL_UNSIGNED_INT; };
    template <>
    struct TypeOption<Float> { const static auto value = GL_FLOAT; };

}

#endif /* wrum_Prim_hpp */
