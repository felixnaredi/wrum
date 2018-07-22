// FunkyArray.hpp
//
// Author: Felix Naredi
// Date: 2018-07-21 21:03:45 +0200
//

#ifndef wrum_FunkyArray_hpp
#define wrum_FunkyArray_hpp

namespace wrum
{
    template <typename T, std::size_t ...Is, std::size_t ...Js>
    constexpr auto concat(
	const std::array<T, sizeof...(Is)>& a1,
	std::index_sequence<Is...>,
	const std::array<T, sizeof...(Js)>& a2,
	std::index_sequence<Js...>)
    {
	return std::array<T, sizeof...(Is) + sizeof...(Js)> { a1[Is]..., a2[Js]... };
    }
    
    template <typename T, std::size_t N1, std::size_t N2>
    constexpr auto concat(std::array<T, N1>&& a1, std::array<T, N2>&& a2)
    {
	return concat(
	    a1,
	    std::make_index_sequence<N1>(),
	    a2,
	    std::make_index_sequence<N2>());
    }	

    template <typename Lambda, std::size_t ...Is>
    constexpr auto map_index_sequence(const Lambda& l, std::index_sequence<Is...>)
    {
	using Return = std::invoke_result_t<Lambda, std::size_t>;
	return std::array<Return, sizeof...(Is)> { l(Is)... };
    }
    
    template <std::size_t N, typename Lambda>
    constexpr auto map_index_sequence(Lambda&& l)
    { return map_index_sequence(l, std::make_index_sequence<N>()); }
}

#endif /* wrum_FunkyArray_hpp */
