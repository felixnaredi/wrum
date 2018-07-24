// VertexBuffer.hpp
//
// Author: Felix Naredi
// Date: 2018-07-17 23:03:36 +0200
//

#ifndef wrum_VertexBuffer_hpp
#define wrum_VertexBuffer_hpp

#include <tuple>
#include <array>

#include "Prim.hpp"
#include "Buffer.hpp"
#include "Program.hpp"

namespace wrum
{
    template <BufferMemory Memory, typename ...Fields>
    class VertexBuffer
    {
	using Self = VertexBuffer<Memory, Fields...>;
	using Tuple = std::tuple<Fields...>;

	const static auto field_count_ = sizeof...(Fields);

	Buffer<Memory, BufferType::Array> buf_;
	Tuple fields_;

	template <std::size_t ...Is>
	constexpr void locate_fields(
	    const Program& prg,
	    std::index_sequence<Is...>)
	noexcept
	{ auto dmy0 = { (std::get<Is>(fields_).locate(prg), 0)... }; }

    public:
	using Vertex = std::tuple<typename Fields::Attrib...>;

	constexpr VertexBuffer(Tuple fields) noexcept
	    : fields_(fields)
	{ }

	constexpr VertexBuffer(Self&& vb) noexcept
	    : buf_(std::move(vb.buf_)),
	      fields_(std::move(vb.fields_))
	{ }

	constexpr void locate_fields(const Program& prg)
	{ locate_fields(prg, std::make_index_sequence<field_count_>()); }

	template <std::size_t N>
    	constexpr void encode(const std::array<Vertex, N>& arr) noexcept
    	{ buf_.encode(arr); }

	constexpr void encode(const std::vector<Vertex>& vec) noexcept
    	{ buf_.encode(vec); }

    private:
	template <std::size_t I, class Field>
    	constexpr void bind_field(const Field& field) const
    	{
    	    glEnableVertexAttribArray(field.location());
    	    Vertex v;
    	    // Tuples (at least on my system) are packed
    	    // backwards. The offset of an Attrib is in other words the
    	    // the position minus the last.
    	    //     If this isn't a general standard there is a risk
    	    // that a custom Tuple class must be implemented.
    	    Field::Attrib::set_ptr(
    		field.location(),
    		sizeof(Vertex),
    		(std::size_t) (
    		    (std::uint8_t*) &std::get<I>(v) -
    		    (std::uint8_t*) &std::get<field_count_ - 1>(v)));
    	}

	template <std::size_t ...Is>
    	constexpr void bind_fields(std::index_sequence<Is...>) const
    	{ auto dmy0 = { (bind_field<Is>(std::get<Is>(fields_)), 0)... }; }

	template <std::size_t ...Is>
	constexpr void disable_fields(std::index_sequence<Is...>) const
	{
	    auto dmy0 = {
		(glDisableVertexAttribArray(std::get<Is>(fields_).location()), 0)...
	    };
	}
    public:
	constexpr void bind()
	{
	    buf_.bind();
	    bind_fields(std::make_index_sequence<field_count_>());
	}

	constexpr void unbind()
	{
	    buf_.unbind();
	    disable_fields(std::make_index_sequence<field_count_>());
	}
    };

    template <BufferMemory Memory, typename ...Fields>
    extern
    constexpr auto make_vertex_buffer(Fields ...fields) noexcept
    { return VertexBuffer<Memory, Fields...>(std::make_tuple(fields...)); }
}

#endif /* wrum_VertexBuffer_hpp */
