// GPURef.hpp
//
// Author: Felix Naredi
// Date: 2018-07-19 03:22:04 +0200
//

#ifndef wrum_GPURef_hpp
#define wrum_GPURef_hpp

#include "Prim.hpp"

namespace wrum
{
    template <typename T>
    class GPURef
    {
	const UInt gid_;
	bool moved_;
    public:
	template <typename ...Args>
	constexpr GPURef(Args ...args) noexcept
	    : gid_(T::create_gpu_ref(args...)),
	      moved_(false)
	{ }
	constexpr GPURef(GPURef&& other) noexcept
	    : gid_(other.gid_),
	      moved_(false)
	{ other.moved_ = true; }

	~GPURef()
	{
	    if(moved_) { return; }
	    T::release_gpu_ref(*this);
	}
	
	GPURef(GPURef& r) = delete;	

	constexpr operator UInt() const { return gid_; }
	constexpr operator const UInt*() const { return &gid_; }
    };
}

#endif /* wrum_GPURef_hpp */
