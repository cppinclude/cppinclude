#pragma once

#include <std_fs>
#include <stdfwd/vector>
#include <stdfwd/string_view>

#include <memory>

//------------------------------------------------------------------------------

namespace cmake_project {
	class IncludesParser;
}

//------------------------------------------------------------------------------

namespace cmake_project::test {

//------------------------------------------------------------------------------

class IncldesParserFixture
{
public:

	using Path		= stdfs::path;
	using Paths		= stdfwd::vector< Path >;

	using Strings	= stdfwd::vector< std::string >;

	IncldesParserFixture();
	~IncldesParserFixture();

	Strings parseString( std::string_view _str );

	static Strings toPathStrings( const Strings & _strings );
	static Strings toStrings( const Paths & _paths );
	static std::string toString( const Path & _path );

private:
	IncludesParser & ensureParser();

private:
	std::unique_ptr< IncludesParser > m_parserPtr;
};

//------------------------------------------------------------------------------

}
