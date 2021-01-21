#pragma once

#include <stdfwd.hpp>

//------------------------------------------------------------------------------

namespace json {
	class JsonValue;

//------------------------------------------------------------------------------

class JsonArray
{
public:

	using ArrayIndex	= std::size_t;
	using JsonValuePtr	= stdfwd::unique_ptr< JsonValue >;

	virtual ~JsonArray() = default;

	virtual ArrayIndex getSize() const = 0;
	virtual bool empty() const = 0;
	virtual JsonValuePtr at( ArrayIndex _index ) const = 0;
};

//------------------------------------------------------------------------------

}
