#pragma once

#include <stdfwd/string_view>

#include <memory>

//------------------------------------------------------------------------------

namespace json
{
class JsonObject;
class JsonAccessor;
}

namespace compilation_db
{
class Accessor;
class Loader;
class Database;
}

namespace compilation_db::test
{
//------------------------------------------------------------------------------

class CompilationDbFixture
{
public:
	CompilationDbFixture();
	~CompilationDbFixture();

	const Database & parseJson( std::string_view _json );

private:
	using JsonObjectPtr = std::unique_ptr< json::JsonObject >;

	JsonObjectPtr createJson( std::string_view _json );
	json::JsonAccessor & ensureJsonAccessor();

	Loader & ensureLoader();
	Accessor & ensureAccessor();

private:
	std::unique_ptr< Database > m_databasePtr;

	std::unique_ptr< Loader > m_loaderPtr;
	std::unique_ptr< Accessor > m_accessorPtr;

	std::unique_ptr< json::JsonAccessor > m_jsonAccessorPtr;
};

//------------------------------------------------------------------------------

}
