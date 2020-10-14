#pragma once

#include <stdfwd.hpp>
#include <memory>
#include <optional>

//------------------------------------------------------------------------------

namespace cxxopts {
	class Options;
	class ParseResult;
}

//------------------------------------------------------------------------------

namespace application {

//------------------------------------------------------------------------------

class ParserArg
{
public:

	ParserArg();
	~ParserArg();

	using Strings			= stdfwd::vector< std::string >;
	using StringOpt			= std::optional< std::string >;
	using StringsOpt		= std::optional< Strings >;

	using IntOpt			= std::optional< int >;
	using BoolOpt			= std::optional< bool >;

	using Path				= stdfs::path;
	using PathOpt			= std::optional< Path >;
	using Paths				= stdfwd::vector< Path >;
	using PathsOpt			= std::optional< Paths >;

	void parse( int _argc, char * _argv[] );

	void addArgument(
		std::string_view _fullname,
		std::string_view _description,
		const std::string & _defaultValue
	);
	void addArgument(
		std::string_view _fullname,
		std::string_view _description,
		const Strings & _defaultValues
	);
	void addArgument(
		std::string_view _fullname,
		std::string_view _description,
		int _defaultValue
	);
	void addArgument(
		std::string_view _fullname,
		std::string_view _description,
		bool _defaultValue
	);
	void addArgument(
		std::string_view _fullname,
		std::string_view _description,
		const Path & _defaultValue
	);
	void addArgument(
		std::string_view _fullname,
		std::string_view _description,
		const Paths & _defaultValues
	);

	void addArgument(
		std::string_view _fullname,
		std::string_view _description
	);

	StringOpt  getArgumentStringValue ( std::string_view _arg ) const;
	StringsOpt getArgumentStringsValue( std::string_view _arg ) const;

	IntOpt  getArgumentIntValue ( std::string_view _arg ) const;
	BoolOpt getArgumentBoolValue( std::string_view _arg ) const;

	PathOpt  getArgumentPathValue ( std::string_view _arg ) const;
	PathsOpt getArgumentPathsValue( std::string_view _arg ) const;

	bool isExistArgument( std::string_view _arg ) const;
	void printHelp( std::ostream & _stream ) const;

private:

	cxxopts::Options & getImpl();
	const cxxopts::Options & getImpl() const;

	template< class _TypeOpt >
	_TypeOpt getArgValue( std::string_view _arg ) const;

	template< class _DefaultValue >
	void addArg(
		std::string_view _fullname,
		std::string_view _description,
		const _DefaultValue & _defaultValues
	);

	static const std::string & toString( const std::string & _str );
	static std::string toString( const Strings & _stringArray );
	static std::string toString( int _number );
	static std::string toString( bool _bool );
	static std::string toString( const Path & _path );

	static std::string removeQuotes( std::string_view _str );
	static Strings removeQuotes( const Strings & _values );

	static Strings toStrings( const Paths & _paths );
	static Paths toPaths( const Strings & _strings );

private:

	std::unique_ptr< cxxopts::Options > m_impl;
	std::unique_ptr< cxxopts::ParseResult > m_resultOpt;

};

//------------------------------------------------------------------------------

}
