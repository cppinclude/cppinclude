#pragma once

#include <stdfwd.hpp>

//------------------------------------------------------------------------------

namespace compilation_db {

//------------------------------------------------------------------------------

class CommandObject
{
public:

	virtual const std::string & getDirectory() const = 0;
	virtual const std::string & getCommand() const = 0;
	virtual const std::string & getFile() const = 0;
};

//------------------------------------------------------------------------------

}
