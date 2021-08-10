#include "application/test/fixtures/app_test_parser_fixture.hpp"

#include "application/tools/app_parser_arg.hpp"

#include "exception/ih/exc_internal_error.hpp"

#include <sstream>
#include <std_fs>
#include <string>
#include <vector>

//------------------------------------------------------------------------------

namespace application::test
{
//------------------------------------------------------------------------------

ParserArgumentsFixture::ParserArgumentsFixture() = default;
ParserArgumentsFixture::~ParserArgumentsFixture() = default;

//------------------------------------------------------------------------------

void ParserArgumentsFixture::addArgument(
	std::string_view _fullname, std::string_view _description )
{
	getPatser().addArgument( _fullname, _description );
}

//------------------------------------------------------------------------------

void ParserArgumentsFixture::addArgument(
	std::string_view _fullname,
	std::string_view _description,
	const std::string & _defaultValue )
{
	getPatser().addArgument( _fullname, _description, _defaultValue );
}

//------------------------------------------------------------------------------

void ParserArgumentsFixture::addArgument(
	std::string_view _fullname,
	std::string_view _description,
	const Strings & _defaultValues )
{
	getPatser().addArgument( _fullname, _description, _defaultValues );
}

//------------------------------------------------------------------------------

void ParserArgumentsFixture::addArgument(
	std::string_view _fullname,
	std::string_view _description,
	int _defaultValue )
{
	getPatser().addArgument( _fullname, _description, _defaultValue );
}

//------------------------------------------------------------------------------

void ParserArgumentsFixture::addArgument(
	std::string_view _fullname,
	std::string_view _description,
	bool _defaultValue )
{
	getPatser().addArgument( _fullname, _description, _defaultValue );
}

//------------------------------------------------------------------------------

void ParserArgumentsFixture::addArgument(
	std::string_view _fullname,
	std::string_view _description,
	const Path & _defaultValue )
{
	getPatser().addArgument( _fullname, _description, _defaultValue );
}

//------------------------------------------------------------------------------

void ParserArgumentsFixture::addArgument(
	std::string_view _fullname,
	std::string_view _description,
	const Paths & _defaultValues )
{
	getPatser().addArgument( _fullname, _description, _defaultValues );
}

//------------------------------------------------------------------------------

void ParserArgumentsFixture::parseArguments( const Strings & _arguments )
{
	Strings argument{ _arguments };
	argument.insert( argument.begin(), " " );

	std::vector< char * > arg;
	arg.reserve( _arguments.size() );
	for( std::string & str: argument )
	{
		arg.push_back( str.data() );
	}

	getPatser().parse( static_cast< int >( arg.size() ), arg.data() );
}

//------------------------------------------------------------------------------

ParserArgumentsFixture::StringOpt
ParserArgumentsFixture::getArgumentStringValue( std::string_view _arg ) const
{
	return getPatser().getArgumentStringValue( _arg );
}

//------------------------------------------------------------------------------

ParserArgumentsFixture::StringOpt
ParserArgumentsFixture::getArgumentStringsValue( std::string_view _arg ) const
{
	StringsOpt stringsOpt{ getPatser().getArgumentStringsValue( _arg ) };
	if( stringsOpt )
	{
		return toString( *stringsOpt );
	}

	return std::nullopt;
}

//------------------------------------------------------------------------------

ParserArgumentsFixture::IntOpt
ParserArgumentsFixture::getArgumentIntValue( std::string_view _arg ) const
{
	return getPatser().getArgumentIntValue( _arg );
}

//------------------------------------------------------------------------------

ParserArgumentsFixture::BoolOpt
ParserArgumentsFixture::getArgumentBoolValue( std::string_view _arg ) const
{
	return getPatser().getArgumentBoolValue( _arg );
}

//------------------------------------------------------------------------------

ParserArgumentsFixture::StringOpt
ParserArgumentsFixture::getArgumentPathValue( std::string_view _arg ) const
{
	PathOpt pathOpt{ getPatser().getArgumentPathValue( _arg ) };
	if( pathOpt )
	{
		return pathOpt->string();
	}

	return std::nullopt;
}

//------------------------------------------------------------------------------

ParserArgumentsFixture::StringOpt
ParserArgumentsFixture::getArgumentPathsValue( std::string_view _arg ) const
{
	PathsOpt pathsOpt{ getPatser().getArgumentPathsValue( _arg ) };
	if( pathsOpt )
	{
		return toString( *pathsOpt );
	}

	return std::nullopt;
}

//------------------------------------------------------------------------------

std::string ParserArgumentsFixture::getHelpString()
{
	std::stringstream stream;
	getPatser().printHelp( stream );
	return stream.str();
}

//------------------------------------------------------------------------------

std::string ParserArgumentsFixture::toString( const Strings & _strings )
{
	const char separator = ',';
	std::string result;
	for( const std::string & str: _strings )
	{
		if( !result.empty() )
		{
			result += separator;
		}
		result += str;
	}
	return result;
}

//------------------------------------------------------------------------------

std::string ParserArgumentsFixture::toString( const Paths & _paths )
{
	const char separator = ',';
	std::string result;
	for( const Path & path: _paths )
	{
		if( !result.empty() )
		{
			result += separator;
		}
		result += path.string();
	}
	return result;
}

//------------------------------------------------------------------------------

bool ParserArgumentsFixture::isExistArgument( std::string_view _argumentName )
{
	return getPatser().isExistArgument( _argumentName );
}

//------------------------------------------------------------------------------

const ParserArg & ParserArgumentsFixture::getPatser() const
{
	INTERNAL_CHECK_ERROR( m_parserPtr );
	return *m_parserPtr;
}

//------------------------------------------------------------------------------

ParserArg & ParserArgumentsFixture::getPatser()
{
	if( !m_parserPtr )
	{
		m_parserPtr = std::make_unique< ParserArg >();
	}

	return *m_parserPtr;
}

//------------------------------------------------------------------------------

}
