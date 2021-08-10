#pragma once

//#include <regex>
#include <re2/re2.h>

//------------------------------------------------------------------------------

namespace tools
{
//------------------------------------------------------------------------------

class Regex
{
public:
	explicit Regex( const std::string & _str );

	Regex( const Regex & _other );

	bool search( const std::string & _str ) const;

	const std::string & toString() const;

private:
	static std::string toString( RE2::ErrorCode _code );

	void checkRegex();

private:
	// const std::regex m_regex;
	const re2::RE2 m_regex;
	const std::string m_str;
};

//------------------------------------------------------------------------------

}
