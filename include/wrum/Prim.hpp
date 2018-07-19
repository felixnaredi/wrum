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

    struct Memory
    {	
	const static auto Stream = GL_STREAM_DRAW;
	const static auto Dynamic = GL_DYNAMIC_DRAW;
	const static auto Static = GL_STATIC_DRAW;

	using OptionT = decltype(Stream);
    };
}

#endif /* wrum_Prim_hpp */
