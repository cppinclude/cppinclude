#include "json/test/fixtures/json_test_fixture.hpp"

#include "json/api/json_object.hpp"
#include "json/ih/json_accessor_impl.hpp"

#include "exception/ih/exc_internal_error.hpp"

//------------------------------------------------------------------------------

namespace json::test {

//------------------------------------------------------------------------------

JsonFixture::JsonFixture() = default;
JsonFixture::~JsonFixture() = default;

//------------------------------------------------------------------------------

void JsonFixture::createJsonFile( std::string_view _text )
{
	m_text << _text;
}

//------------------------------------------------------------------------------

const JsonObject & JsonFixture::loadJson()
{
	JsonAccessor & accessor = ensureJsonAccessor();
	auto newJson = accessor.createJson( m_text );
	INTERNAL_CHECK_WARRING( newJson );
	m_json.swap( newJson );
	INTERNAL_CHECK_ERROR( m_json );
	return *m_json;
}

//------------------------------------------------------------------------------

JsonAccessor & JsonFixture::ensureJsonAccessor()
{
	if( !m_accessor )
		m_accessor.reset( new JsonAccesorImpl );

	return *m_accessor;
}

//------------------------------------------------------------------------------

}
