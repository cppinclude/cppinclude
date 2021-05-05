#pragma once

#include <stdfwd/memory>

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
