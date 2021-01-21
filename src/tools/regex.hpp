#pragma once

#include <regex>

//------------------------------------------------------------------------------

namespace tools {

//------------------------------------------------------------------------------

class Regex
{
public:

	explicit Regex( const std::string & _str );
	Regex( const Regex & _other );

	bool search( const std::string & _str ) const;

	const std::string & toString() const;

private:

	const std::regex m_regex;
	const std::string m_str;

};

//------------------------------------------------------------------------------

}
