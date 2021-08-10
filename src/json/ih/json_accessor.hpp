#pragma once

#include <iosfwd>
#include <stdfwd/memory>

//------------------------------------------------------------------------------

namespace json
{
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
