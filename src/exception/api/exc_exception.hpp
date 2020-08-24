#pragma once

#include <stdfwd.hpp>
#include <exception>

//------------------------------------------------------------------------------

namespace exception {

//------------------------------------------------------------------------------

class Exception : public std::exception
{
public:

	virtual ~Exception() = default;

	virtual std::string getModuleName() const noexcept = 0;
	virtual std::string getCode() const noexcept = 0;
	virtual std::string getMessage() const noexcept = 0;

};

//------------------------------------------------------------------------------

}
