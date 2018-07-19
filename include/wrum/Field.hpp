// Field.hpp
//
// Author: Felix Naredi
// Date: 2018-07-19 17:55:14 +0200
//

#ifndef wrum_Field_hpp
#define wrum_Field_hpp

#include <optional>
#include <sstream>

#include "Attrib.hpp"
#include "Program.hpp"

namespace wrum
{
    template <typename AttribT>
    class LabelField
    {       	
	const std::string label_;
	std::optional<Int> loc_;

	auto exception(const std::string&& descr) const noexcept
	{
	    std::stringstream ss;
	    ss << "Field with label '"
	       << label_
	       << "' - " << descr;
	    return ss.str();
	}
    public:
	using Attrib = AttribT;

	LabelField(const std::string&& label) noexcept
	    : label_(label)
	{ }
	
	constexpr auto location() const
	{
	    if(loc_.has_value() == false) {
		throw std::runtime_error(
		    exception("Cannot get location of unlocated field"));
	    }
	    return *loc_;
	}

	void locate(const Program& prg)
	{
	    if(prg.is_used() == false) {
		throw std::runtime_error(
		    exception("Program must be used when locating"));
	    }
	    auto loc = glGetAttribLocation(prg.ref(), label_.c_str());
	    if(loc < 0) {
		throw std::runtime_error(
		    exception("Unable to find field"));
	    }
	    loc_ = loc;
	}	
    };
}

#endif /* wrum_Field_hpp */
