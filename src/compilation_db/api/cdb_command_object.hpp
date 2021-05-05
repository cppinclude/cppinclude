#pragma once

#include <stdfwd/string>

//------------------------------------------------------------------------------

namespace compilation_db {

//------------------------------------------------------------------------------

class CommandObject
{
public:
	virtual ~CommandObject() = default;

	virtual const std::string & getDirectory() const = 0;
	virtual const std::string & getCommand() const = 0;
	virtual const std::string & getFile() const = 0;
};

//------------------------------------------------------------------------------

}
