#pragma once

#include <stdfwd/memory>
#include <iosfwd>

//------------------------------------------------------------------------------

namespace json {
	class JsonObject;

//------------------------------------------------------------------------------

class JsonAccessor
{
public:
	using JsonObjectPtr = stdfwd::unique_ptr< JsonObject >;

	virtual ~JsonAccessor() = default;

	virtual JsonObjectPtr createJson( std::istream & _stream ) = 0;
};

//------------------------------------------------------------------------------

}
