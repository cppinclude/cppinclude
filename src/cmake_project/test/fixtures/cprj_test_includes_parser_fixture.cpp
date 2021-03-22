#include "cmake_project/test/fixtures/cprj_test_includes_parser_fixture.hpp"

#include "cmake_project/impl/cprj_includes_parser.hpp"

#include "tools/path_string_tools.hpp"

#include <std_fs>
#include <string>
#include <vector>

//------------------------------------------------------------------------------

namespace cmake_project::test {

//------------------------------------------------------------------------------

IncldesParserFixture::IncldesParserFixture() = default;
IncldesParserFixture::~IncldesParserFixture() = default;

//------------------------------------------------------------------------------

IncldesParserFixture::Strings IncldesParserFixture::parseString(
	std::string_view _str
)
{
	Paths includes = ensureParser().parse( _str );
	return toStrings( includes );
}

//------------------------------------------------------------------------------

IncldesParserFixture::Strings IncldesParserFixture::toPathStrings(
	const Strings & _strings
)
{
	Strings result;
	for( const std::string & str : _strings )
	{
		std::string newStr = tools::toPath( str );
		result.push_back( newStr );
	}

	return result;
}

//------------------------------------------------------------------------------

IncludesParser & IncldesParserFixture::ensureParser()
{
	if( !m_parserPtr )
	{
		m_parserPtr = std::make_unique< IncludesParser >();
	}

	return *m_parserPtr;
}

//------------------------------------------------------------------------------

IncldesParserFixture::Strings IncldesParserFixture::toStrings(
	const Paths & _paths
)
{
	Strings result;
	for( const Path & path : _paths )
	{
		const std::string str = toString( path );
		result.push_back( str );
	}
	return result;
}

//------------------------------------------------------------------------------

std::string IncldesParserFixture::toString( const Path & _path )
{
	return tools::toPath( _path.string() );
}

//------------------------------------------------------------------------------

}
