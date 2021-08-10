#include "tools/regex.hpp"

#include "tools/regex_exception.hpp"

#include <regex>

//------------------------------------------------------------------------------

namespace tools
{
//------------------------------------------------------------------------------

Regex::Regex( const std::string & _str )
	: m_regex{ _str }
	, m_str{ _str }
{
	checkRegex();
}

//------------------------------------------------------------------------------

Regex::Regex( const Regex & _other )
	: Regex( _other.m_str )
{
}

//------------------------------------------------------------------------------

bool Regex::search( const std::string & _str ) const
{
	// return  std::regex_search( _str, m_regex );
	const bool result = RE2::PartialMatch( _str, m_regex );
	return result;
}

//------------------------------------------------------------------------------

const std::string & Regex::toString() const
{
	return m_str;
}

//------------------------------------------------------------------------------

std::string Regex::toString( RE2::ErrorCode _code )
{
	switch( _code )
	{
		case RE2::ErrorCode::ErrorInternal:
			return "Unexpected error";
		case RE2::ErrorCode::ErrorBadEscape:
			return "bad escape sequence";
		case RE2::ErrorCode::ErrorBadCharClass:
			return "bad character class";
		case RE2::ErrorCode::ErrorBadCharRange:
			return "bad character class range";
		case RE2::ErrorCode::ErrorMissingBracket:
			return "missing closing ]";
		case RE2::ErrorCode::ErrorMissingParen:
			return "missing closing )";
		case RE2::ErrorCode::ErrorUnexpectedParen:
			return "unexpected closing )";
		case RE2::ErrorCode::ErrorTrailingBackslash:
			return "trailing \\ at end of regexp";
		case RE2::ErrorCode::ErrorRepeatArgument:
			return "repeat argument missing, e.g. \"*\"";
		case RE2::ErrorCode::ErrorRepeatSize:
			return "bad repetition argument";
		case RE2::ErrorCode::ErrorRepeatOp:
			return "bad repetition operator";
		case RE2::ErrorCode::ErrorBadPerlOp:
			return "bad perl operator";
		case RE2::ErrorCode::ErrorBadUTF8:
			return "invalid UTF-8 in regexp";
		case RE2::ErrorCode::ErrorBadNamedCapture:
			return "bad named capture group";
		case RE2::ErrorCode::ErrorPatternTooLarge:
			return "pattern too large (compile failed)";
		default:
			return "";
	}
}

//------------------------------------------------------------------------------

void Regex::checkRegex()
{
	const RE2::ErrorCode code = m_regex.error_code();
	if( code != RE2::ErrorCode::NoError )
	{
		throw RegexException( m_str, toString( code ) );
	}
}

//------------------------------------------------------------------------------

}
