#include "application/tools/app_parser_arg.hpp"

#include "exception/ih/exc_internal_error.hpp"

#include <cxxopts.hpp>

#include <string>
#include <optional>
#include <vector>
#include <filesystem>

//------------------------------------------------------------------------------

namespace application {

//------------------------------------------------------------------------------

ParserArg::ParserArg() = default;
ParserArg::~ParserArg() = default;

//------------------------------------------------------------------------------

void ParserArg::parse( int _argc, char * _argv[] )
{
	try
	{
		m_resultOpt = std::make_unique< cxxopts::ParseResult >(
			getImpl().parse( _argc, _argv )
		);
	}
	catch ( const cxxopts::option_not_exists_exception & _exception )
	{
		throw std::logic_error{ _exception.what() };
	}
}

//------------------------------------------------------------------------------

void ParserArg::addArgument(
	std::string_view _fullname,
	std::string_view _description,
	const std::string & _defaultValue
)
{
	addArg( _fullname, _description, _defaultValue );
}

//------------------------------------------------------------------------------

void ParserArg::addArgument(
	std::string_view _fullname,
	std::string_view _description,
	const Strings & _defaultValues
)
{
	addArg( _fullname, _description, _defaultValues );
}

//------------------------------------------------------------------------------

void ParserArg::addArgument(
	std::string_view _fullname,
	std::string_view _description,
	int _defaultValue
)
{
	addArg( _fullname, _description, _defaultValue );
}

//------------------------------------------------------------------------------

void ParserArg::addArgument(
	std::string_view _fullname,
	std::string_view _description,
	bool _defaultValue
)
{
	addArg( _fullname, _description, _defaultValue );
}

//------------------------------------------------------------------------------

void ParserArg::addArgument(
	std::string_view _fullname,
	std::string_view _description,
	const Path & _defaultValue
)
{
	addArg( _fullname, _description, toString( _defaultValue ) );
}

//------------------------------------------------------------------------------

void ParserArg::addArgument(
	std::string_view _fullname,
	std::string_view _description,
	const Paths & _defaultValues
)
{
	Strings strings{ toStrings( _defaultValues ) };
	addArg( _fullname, _description, strings );
}

//------------------------------------------------------------------------------

template< class _DefaultValue >
void ParserArg::addArg(
	std::string_view _fullname,
	std::string_view _description,
	const _DefaultValue & _defaultValues
)
{
	const std::string fullName{ _fullname };
	const std::string description{ _description };

	auto str{ toString( _defaultValues ) };
	auto value{ cxxopts::value<_DefaultValue>()->default_value( str) };
	getImpl().add_options()( fullName, description, value );
}

//------------------------------------------------------------------------------

void ParserArg::addArgument(
	std::string_view _fullname,
	std::string_view _description
)
{
	getImpl().add_options()(
		std::string( _fullname ),
		std::string( _description )
	);
}

//------------------------------------------------------------------------------

ParserArg::StringOpt ParserArg::getArgumentStringValue(
	std::string_view _arg
) const
{
	return getArgValue< StringOpt >( _arg );
}

//------------------------------------------------------------------------------

ParserArg::StringsOpt ParserArg::getArgumentStringsValue(
	std::string_view _arg
) const
{
	return getArgValue< StringsOpt >( _arg );
}

//------------------------------------------------------------------------------

ParserArg::IntOpt ParserArg::getArgumentIntValue( std::string_view _arg ) const
{
	return getArgValue< IntOpt >( _arg );
}

//------------------------------------------------------------------------------

ParserArg::BoolOpt ParserArg::getArgumentBoolValue( std::string_view _arg ) const
{
	return getArgValue< BoolOpt >( _arg );
}

//------------------------------------------------------------------------------

ParserArg::PathOpt ParserArg::getArgumentPathValue( std::string_view _arg ) const
{
	if( StringOpt stirngOpt = getArgumentStringValue( _arg ); stirngOpt )
		return PathOpt{ *stirngOpt };

	return std::nullopt;
}

//------------------------------------------------------------------------------

ParserArg::PathsOpt ParserArg::getArgumentPathsValue( std::string_view _arg ) const
{
	if( StringsOpt stringsOpt = getArgumentStringsValue( _arg ); stringsOpt )
		return toPaths( *stringsOpt );

	return std::nullopt;
}

//------------------------------------------------------------------------------

cxxopts::Options & ParserArg::getImpl()
{
	if( !m_impl )
		m_impl.reset( new cxxopts::Options{ "" } ) ;

	return *m_impl;
}

//------------------------------------------------------------------------------

const cxxopts::Options & ParserArg::getImpl() const
{
	if( !m_impl )
	{
		THROW_INTERNAL_ERROR
	}
	return *m_impl;
}

//------------------------------------------------------------------------------

template< class _TypeOpt >
_TypeOpt ParserArg::getArgValue( std::string_view _arg ) const
{
	using value_type = typename _TypeOpt::value_type;
	if( isExistArgument( _arg ) )
	{
		std::string argumentName{ _arg };
		value_type value = (*m_resultOpt)[argumentName].as< value_type >();
		if constexpr(
			std::is_same_v< _TypeOpt , StringOpt > ||
			std::is_same_v< _TypeOpt , StringsOpt >
		)
		{
			return removeQuotes( value );
		}
		else
		{
			return value;
		}
	}

	return std::nullopt;
}

//------------------------------------------------------------------------------

bool ParserArg::isExistArgument( std::string_view _arg ) const
{
	if( m_resultOpt )
	{
		std::string argumentName{ _arg };
		const bool found = m_resultOpt->count( argumentName );
		return found;
	}
	return false;
}

//------------------------------------------------------------------------------

void ParserArg::printHelp( std::ostream & _stream ) const
{
	_stream << getImpl().help();
}

//------------------------------------------------------------------------------

std::string ParserArg::toString( const Strings & _stringArray )
{
	std::string result = "";
	const size_t size = _stringArray.size();
	for( size_t i = 0; i < size ; ++i)
	{
		result += _stringArray[i];
		if( i != size - 1 )
			result += ',';
	}
	return result;
}

//------------------------------------------------------------------------------

const std::string & ParserArg::toString( const std::string & _str )
{
	return _str;
}

//------------------------------------------------------------------------------

std::string ParserArg::toString( int _number )
{
	return std::to_string( _number );
}

//------------------------------------------------------------------------------

std::string ParserArg::toString( bool _bool )
{
	return _bool ? "true" : "false";
}

//------------------------------------------------------------------------------

std::string ParserArg::toString( const Path & _path )
{
	return _path.string();
}

//------------------------------------------------------------------------------

std::string ParserArg::removeQuotes( std::string_view _str )
{
	if( _str.size() < 3 )
		return std::string{ _str };

	const char firstChar = _str.at( 0 );
	const char lastChar = _str.at( _str.size() - 1 );
	if( firstChar == lastChar && ( firstChar == '\'' || firstChar == '"' ) )
	{
		std::string_view result = _str.substr( 1, _str.size() - 2 );
		return std::string{ result };
	}
	else
	{
		return std::string{ _str };
	}
}

//------------------------------------------------------------------------------

ParserArg::Strings ParserArg::removeQuotes( const Strings & _values )
{
	Strings newValues;
	newValues.reserve( _values.size() );
	for( std::string_view value : _values )
	{
		std::string newValue = removeQuotes( value );
		newValues.push_back( newValue );
	}
	return newValues;
}

//------------------------------------------------------------------------------

ParserArg::Strings ParserArg::toStrings( const Paths & _paths )
{
	Strings result;
	result.reserve( _paths.size() );
	for( const Path & path : _paths )
	{
		result.push_back( toString( path ) );
	}
	return result;
}

//------------------------------------------------------------------------------

ParserArg::Paths ParserArg::toPaths( const Strings & _strings )
{
	Paths result;
	result.reserve( _strings.size() );
	for( const std::string & str : _strings )
	{
		result.push_back( str );
	}
	return result;
}

//------------------------------------------------------------------------------

}
