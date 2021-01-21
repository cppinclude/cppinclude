#pragma once

#include "json/api/json_value.hpp"

#include <nlohmann/json.hpp>

//------------------------------------------------------------------------------

namespace json {

//------------------------------------------------------------------------------

class JsonValueImpl final : public JsonValue
{
public:

	using JsonImpl = nlohmann::json;

	JsonValueImpl( JsonImpl && _jsonImpl );

	std::string asString() const override;
	int asInt() const override;
	bool asBool() const override;

	JsonObjectPtr asObject() const override;
	JsonArrayPtr asArray() const override;

private:

	JsonImpl m_jsonImpl;

};

//------------------------------------------------------------------------------

}
