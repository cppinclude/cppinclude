#pragma once

#include "json/api/json_object.hpp"

#include <nlohmann/json.hpp>

//------------------------------------------------------------------------------

namespace json {

//------------------------------------------------------------------------------

class JsonObjectImpl final : public JsonObject
{

public:

	using JsonObjectPtr = std::unique_ptr< JsonObject >;
	using JsonImpl = nlohmann::json;

	static JsonObjectPtr createJson( std::istream & _stream );

	explicit JsonObjectImpl( JsonImpl && _jsonImpl );
	explicit JsonObjectImpl( std::istream & _stream );

	JsonValuePtr getAttributeValue( std::string_view _name ) const override;
	JsonValuePtr asValue() const override;

private:

	JsonImpl m_jsonImpl;
};

//------------------------------------------------------------------------------

}
