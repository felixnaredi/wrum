// VertexBuffer.hpp
//
// Author: Felix Naredi
// Date: 2018-07-17 23:03:36 +0200
//

#ifndef wrum_VertexBuffer_hpp
#define wrum_VertexBuffer_hpp

#include <cstdint>
#include <tuple>
#include <stdexcept>

#include <GL/glew.h>

#include "Prim.hpp"
#include "Program.hpp"

namespace wrum
{
    template <GLenum Memory, typename ...Attribs>
    struct VertexBuffer    
    {
	using Vertex = std::tuple<Attribs...>;
    private:
	const static auto buf_t = GL_ARRAY_BUFFER;
	const static auto attrib_count = sizeof...(Attribs);

	const UInt gpu_ref_;
	std::array<Int, attrib_count> locs_;
	
	template <typename Lambda, std::size_t ...Is>
	constexpr static auto map_indicies(Lambda l, std::index_sequence<Is...>)
	-> std::array<Vertex, sizeof...(Is)>
	{
	    return { l(Is)... };
	}

	constexpr void bind() const noexcept { glBindBuffer(buf_t, gpu_ref_); }
	constexpr void unbind() const noexcept { glBindBuffer(buf_t, 0); }

	static auto gen() noexcept
	{
	    UInt id;
	    glGenBuffers(1, &id);
	    return id;
	}

	template <std::size_t I, typename Attrib>
	void set_attrib() const noexcept
	{	    
	    glEnableVertexAttribArray(locs_[I]);
	    Vertex v;
	    // Tuples (at least on my system) are packed
	    // backwards. The offset of an Attrib is in other words the
	    // the position minus the last.
	    //     If this isn't a general standard there is a risk
	    // that a custom Tuple class must be implemented.
	    Attrib::setp(
		locs_[I],
		sizeof(Vertex),
		(std::size_t) (
		    (std::uint8_t*) &std::get<I>(v) -
		    (std::uint8_t*) &std::get<attrib_count - 1>(v)));
	}
	
	template <std::size_t ...Is>
	void set_attribs(std::index_sequence<Is...>) const noexcept
	{
	    auto dummy = { (set_attrib<Is, Attribs>(), 0)... };
	}
	
    public:
	VertexBuffer() : gpu_ref_(gen()) { }

	void locate_attribs(const Program& prg)
	{
	    decltype(locs_) a { Attribs::locate(prg)... };
	    std::swap(locs_, a);
	}
	
	constexpr void enable() const noexcept
	{
	    bind();
	    set_attribs(std::make_index_sequence<attrib_count>());	    
	}

	constexpr void disable() const noexcept
	{
	    for(auto& loc : locs_) { glDisableVertexAttribArray(loc); }
	}

	template <std::size_t N>
	void encode(const std::array<Vertex, N>& data) const noexcept
	{
	    bind();
	    glBufferData(buf_t, N * sizeof(Vertex), &data, Memory);
	    unbind();
	}
	
	template <std::size_t N, typename Lambda>
	void encode(Lambda l) const
	{
	    encode(std::forward<std::array<Vertex, N>>(
		map_indicies(l, std::make_index_sequence<N>())));
	}
    };
}

#endif /* wrum_VertexBuffer_hpp */
