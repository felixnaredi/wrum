// VertexBuffer.hpp
//
// Author: Felix Naredi
// Date: 2018-07-17 23:03:36 +0200
//

#ifndef wrum_VertexBuffer_hpp
#define wrum_VertexBuffer_hpp

#include <cstdint>
#include <tuple>
#include <atomic>

#include <GL/glew.h>

#include "Prim.hpp"
#include "GPURef.hpp"
#include "Program.hpp"

namespace wrum
{
    template <Memory::OptionT MemoryOption, typename Fields>
    struct VertexBuffer    
    { };

    template <Memory::OptionT MemoryOption, typename ...Fields>
    class VertexBuffer<MemoryOption, std::tuple<Fields...>>
    {	
	using Tuple = std::tuple<Fields...>;
	using Self = VertexBuffer<MemoryOption, Tuple>;

	const static auto buf_t = GL_ARRAY_BUFFER;
	const static auto field_count_ = sizeof...(Fields);

	template <typename Lambda, std::size_t ...Is>
    	constexpr static auto map_indicies(const Lambda& l, std::index_sequence<Is...>)
    	{ return std::array<Vertex, sizeof...(Is)>{ l(Is)... }; }

	const GPURef<Self> gpu_ref_;
	std::atomic_bool bound_;
	Tuple fields_;	

	constexpr void bind() noexcept
	{
	    if(bound_.load()) { return; }
	    glBindBuffer(buf_t, gpu_ref_);
	    bound_.store(true);
	}
	
    	constexpr void unbind() noexcept
	{
	    if(bound_.load() == false) { return; }
	    glBindBuffer(buf_t, 0);
	    bound_.store(false);
	}

	template <std::size_t ...Is>
	constexpr void locate_fields(
	    const Program& prg,
	    std::index_sequence<Is...>)
	noexcept	
	{ auto __dmy = { (std::get<Is>(fields_).locate(prg), 0)... }; }

    public:
	using Vertex = std::tuple<typename Fields::Attrib...>;

	static auto create_gpu_ref() noexcept
    	{
    	    UInt id;
    	    glGenBuffers(1, &id);
    	    return id;
    	}

	static auto release_gpu_ref(const GPURef<Self>& ref) noexcept
	{ glDeleteBuffers(1, ref); }
	
	constexpr VertexBuffer(Tuple fields) noexcept
	    : fields_(fields),
	      bound_(false)
	{ }	

	void locate_fields(const Program& prg)
	{ locate_fields(prg, std::make_index_sequence<field_count_>()); }

		
	template <std::size_t N>
    	void encode(const std::array<Vertex, N>& data) noexcept
    	{
    	    bind();
    	    glBufferData(buf_t, N * sizeof(Vertex), &data, MemoryOption);
    	    unbind();
    	}

	template <std::size_t N, typename Lambda>
    	constexpr void encode(const Lambda& l)
    	{
    	    encode(std::forward<std::array<Vertex, N>>(
    		map_indicies(l, std::make_index_sequence<N>())));
    	}

	template <std::size_t N, typename Lambda>
	constexpr void encode(Lambda&& l) { encode<N>(l); }
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
    	constexpr void bind_fields(std::index_sequence<Is...>) const noexcept
    	{ auto __dmy = { (bind_field<Is>(std::get<Is>(fields_)), 0)... }; }
	
    public:

	void use()
	{
	    bind();
	    bind_fields(std::make_index_sequence<field_count_>());
	}

	void unuse()
	{
	    unbind();
	    unbind_fields(std::make_index_sequence<field_count_>());
	}
    };

    template <Memory::OptionT MemoryOption, typename ...Fields>
    inline
    constexpr auto makeVertexBuffer(Fields ...fields)
    {
	return VertexBuffer<MemoryOption,std::tuple<Fields...>>(
	    std::make_tuple(fields...));
    }
}

#endif /* wrum_VertexBuffer_hpp */
