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
    class GPURef
    {
	const UInt gid_;	
    public:
	constexpr GPURef(UInt gid) noexcept : gid_(gid) { }
	constexpr GPURef(const GPURef&& r) noexcept : GPURef(r.gid_) { }
	GPURef(GPURef& r) = delete;	

	constexpr operator UInt() const { return gid_; }
	constexpr operator const UInt*() const { return &gid_; }
    };
}

#endif /* wrum_GPURef_hpp */
