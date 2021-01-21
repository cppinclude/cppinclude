#pragma once

#include "json/api/json_array.hpp"

#include <nlohmann/json.hpp>

//------------------------------------------------------------------------------

namespace json {

//------------------------------------------------------------------------------

class JsonArrayImpl final : public JsonArray
{
public:

	using JsonImpl = nlohmann::json;

	JsonArrayImpl( JsonImpl && _jsonImpl );

	ArrayIndex getSize() const override;
	bool empty() const override;
	JsonValuePtr at( ArrayIndex _index ) const override;

private:

	JsonImpl m_jsonImpl;
};

//------------------------------------------------------------------------------

}
