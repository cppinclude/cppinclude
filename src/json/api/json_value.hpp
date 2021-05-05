#pragma once

#include <stdfwd/memory>
#include <stdfwd/string>

//------------------------------------------------------------------------------

namespace json {
	class JsonObject;
	class JsonArray;

//------------------------------------------------------------------------------

class JsonValue
{
public:

	using JsonObjectPtr		= stdfwd::unique_ptr< JsonObject >;
	using JsonArrayPtr		= stdfwd::unique_ptr< JsonArray >;

	virtual ~JsonValue() = default;

	virtual std::string asString() const = 0;
	virtual int asInt() const = 0;
	virtual bool asBool() const = 0;

	virtual JsonObjectPtr asObject() const = 0;
	virtual JsonArrayPtr asArray() const = 0;
};

//------------------------------------------------------------------------------

}
