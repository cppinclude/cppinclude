#include "compilation_db/test/fixtures/cdb_test_fixture.hpp"

#include "compilation_db/api/cdb_database.hpp"
#include "compilation_db/api/cdb_loader.hpp"
#include "compilation_db/ih/cdb_accessor_impl.hpp"

#include "exception/ih/exc_internal_error.hpp"

#include "json/api/json_object.hpp"
#include "json/ih/json_accessor_impl.hpp"

#include <sstream>
#include <string_view>

//------------------------------------------------------------------------------

namespace compilation_db::test {

//------------------------------------------------------------------------------

CompilationDbFixture::CompilationDbFixture() = default;
CompilationDbFixture::~CompilationDbFixture() = default;

//------------------------------------------------------------------------------

const Database & CompilationDbFixture::parseJson( std::string_view _json )
{
	auto jsonPtr = createJson( _json );
	INTERNAL_CHECK_ERROR( jsonPtr )

	m_databasePtr = ensureLoader().load( *jsonPtr );

	INTERNAL_CHECK_ERROR( m_databasePtr );
	return *m_databasePtr;
}

//------------------------------------------------------------------------------

CompilationDbFixture::JsonObjectPtr CompilationDbFixture::createJson(
	std::string_view _json
)
{
	std::stringstream stream;
	stream << _json;
	return ensureJsonAccessor().createJson( stream );
}

//------------------------------------------------------------------------------

json::JsonAccessor & CompilationDbFixture::ensureJsonAccessor()
{
	if( !m_jsonAccessorPtr )
	{
		m_jsonAccessorPtr = std::make_unique< json::JsonAccesorImpl >();
	}

	return *m_jsonAccessorPtr;
}

//------------------------------------------------------------------------------

Loader & CompilationDbFixture::ensureLoader()
{
	if( !m_loaderPtr )
	{
		m_loaderPtr = ensureAccessor().createLoader();
	}

	return *m_loaderPtr;
}

//------------------------------------------------------------------------------

Accessor & CompilationDbFixture::ensureAccessor()
{
	if( !m_accessorPtr )
	{
		m_accessorPtr = std::make_unique< AccessorImpl >();
	}

	return *m_accessorPtr;
}

//------------------------------------------------------------------------------

}
