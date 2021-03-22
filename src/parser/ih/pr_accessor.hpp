#pragma once

#include <stdfwd.hpp>

//------------------------------------------------------------------------------

namespace parser {
	class Parser;

//------------------------------------------------------------------------------

class ParserAccessor
{
public:
	virtual ~ParserAccessor() = default;

	using ParserPtr = stdfwd::unique_ptr< Parser >;

	virtual ParserPtr createParser() = 0;
};

//------------------------------------------------------------------------------

}
