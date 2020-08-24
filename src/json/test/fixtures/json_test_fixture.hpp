#pragma once

#include <stdfwd.hpp>
#include <sstream>
#include <memory>

//------------------------------------------------------------------------------

namespace json {
	class JsonObject;
	class JsonAccessor;
}

//------------------------------------------------------------------------------

namespace json::test {

//------------------------------------------------------------------------------

class JsonFixture
{
public:

	JsonFixture();
	~JsonFixture();

	void createJsonFile( std::string_view _text );
	const JsonObject & loadJson();

private:

	JsonAccessor & ensureJsonAccessor();

private:

	std::unique_ptr< JsonAccessor > m_accessor;
	std::unique_ptr< JsonObject > m_json;
	std::stringstream m_text;
};

//------------------------------------------------------------------------------

}
