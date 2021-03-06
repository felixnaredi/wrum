// Buffer.hpp
//
// Author: Felix Naredi
// Date: 2018-07-21 17:02:20 +0200
//

#ifndef wrum_Buffer_hpp
#define wrum_Buffer_hpp

#include <array>
#include <atomic>
#include <vector>

#include "Prim.hpp"
#include "GPURef.hpp"

namespace wrum
{
    enum class BufferMemory
    {
	Stream = GL_STREAM_DRAW,
	Dynamic = GL_DYNAMIC_DRAW,
	Static = GL_STATIC_DRAW,
    };

    enum class BufferType
    {
	Array = GL_ARRAY_BUFFER,
	ElementArray = GL_ELEMENT_ARRAY_BUFFER,
    };

    template <BufferMemory Memory, BufferType BufT>
    class Buffer
    {
	using Self = Buffer<Memory, BufT>;

	GPURef<Self> gpu_ref_;
	std::atomic_bool bound_;
    public:
	constexpr static auto create_gpu_ref() noexcept
	{
	    UInt id(0);
	    glGenBuffers(1, &id);
	    return id;
	}

	constexpr static auto release_gpu_ref(const GPURef<Self>& ref) noexcept
	{ glDeleteBuffers(1, ref); }

	constexpr Buffer() noexcept : bound_(false) { }

	Buffer(Self&& buf) noexcept
	    : bound_(buf.bound_.load()),
	      gpu_ref_(std::move(buf.gpu_ref_))
	{ }

	constexpr void bind() noexcept
	{
	    if(bound_.load()) { return; }
	    glBindBuffer(static_cast<Option>(BufT), gpu_ref_);
	    bound_.store(true);
	}

    	constexpr void unbind() noexcept
	{
	    if(bound_.load() == false) { return; }
	    glBindBuffer(static_cast<Option>(BufT), 0);
	    bound_.store(false);
	}

	template <typename T>
	constexpr void encode(const T* data, std::size_t size) noexcept
	{
	    bind();
	    glBufferData(
		static_cast<Option>(BufT),
		size,
		data,
		static_cast<Option>(Memory));
	    unbind();
	}

	template <typename T, std::size_t N>
	constexpr void encode(const std::array<T, N>& arr) noexcept
	{ encode(arr.data(), N * sizeof(T)); }

	template <typename T>
	constexpr void encode(const std::vector<T>& vec) noexcept
	{ encode(vec.data(), vec.size() * sizeof(T)); }
    };
}

#endif /* wrum_Buffer_hpp */
