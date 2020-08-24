#pragma once

#include <stdfwd.hpp>

//------------------------------------------------------------------------------

namespace json {
	class JsonValue;

//------------------------------------------------------------------------------

class JsonObject
{
public:

	using JsonValuePtr = stdfwd::unique_ptr< JsonValue >;

	virtual ~JsonObject() = default;

	virtual JsonValuePtr getAttributeValue( std::string_view _name ) const = 0;
};

//------------------------------------------------------------------------------

}
