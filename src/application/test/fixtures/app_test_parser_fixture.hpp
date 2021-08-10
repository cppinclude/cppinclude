#pragma once

#include <std_fs>
#include <stdfwd/optional>
#include <stdfwd/string_view>
#include <stdfwd/vector>

#include <memory>

//------------------------------------------------------------------------------

namespace application
{
class ParserArg;
}

//------------------------------------------------------------------------------

namespace application::test
{
//------------------------------------------------------------------------------

class ParserArgumentsFixture
{
public:
	ParserArgumentsFixture();
	~ParserArgumentsFixture();

	using Strings = stdfwd::vector< std::string >;
	using StringOpt = std::optional< std::string >;
	using StringsOpt = std::optional< Strings >;

	using IntOpt = std::optional< int >;
	using BoolOpt = std::optional< bool >;

	using Path = stdfs::path;
	using PathOpt = std::optional< Path >;
	using Paths = stdfwd::vector< Path >;
	using PathsOpt = std::optional< Paths >;

	void
	addArgument( std::string_view _fullname, std::string_view _description );

	void addArgument(
		std::string_view _fullname,
		std::string_view _description,
		const std::string & _defaultValue );

	void addArgument(
		std::string_view _fullname,
		std::string_view _description,
		const Strings & _defaultValues );

	void addArgument(
		std::string_view _fullname,
		std::string_view _description,
		int _defaultValue );

	void addArgument(
		std::string_view _fullname,
		std::string_view _description,
		bool _defaultValue );

	void addArgument(
		std::string_view _fullname,
		std::string_view _description,
		const Path & _defaultValue );

	void addArgument(
		std::string_view _fullname,
		std::string_view _description,
		const Paths & _defaultValues );

	void parseArguments( const Strings & _arguments );

	StringOpt getArgumentStringValue( std::string_view _arg ) const;
	StringOpt getArgumentStringsValue( std::string_view _arg ) const;

	IntOpt getArgumentIntValue( std::string_view _arg ) const;
	BoolOpt getArgumentBoolValue( std::string_view _arg ) const;

	StringOpt getArgumentPathValue( std::string_view _arg ) const;
	StringOpt getArgumentPathsValue( std::string_view _arg ) const;

	bool isExistArgument( std::string_view _argumentName );

	std::string getHelpString();

	static std::string toString( const Strings & _strings );
	static std::string toString( const Paths & _paths );

private:
	const ParserArg & getPatser() const;
	ParserArg & getPatser();

private:
	std::unique_ptr< ParserArg > m_parserPtr;
};

//------------------------------------------------------------------------------

}
