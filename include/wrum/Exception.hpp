// Exception.hpp
//
// Author: Felix Naredi
// Date: 2018-07-18 23:56:28 +0200
//

#ifndef wrum_Exception_hpp
#define wrum_Exception_hpp

#include <string>
#include <ostream>

namespace wrum
{
    template <typename Get>
    struct Log
    {
	const std::size_t len;
	const Get get;
	constexpr Log(const std::size_t _len, Get&& _get) noexcept
	    : len(_len),
	      get(_get)
	{ }
    };

    template <typename CharT, typename Get>
    auto& operator<<(std::basic_ostream<CharT>& os, const Log<Get>&& log)
    {
	std::string s(log.len + 1, '\0');
	log.get(s);
	return os << s.c_str();
    }

    struct Exception
    { };
}

#endif /* wrum_Exception_hpp */
