#include "tools/regex_exception.hpp"

//------------------------------------------------------------------------------

namespace tools
{
//------------------------------------------------------------------------------

RegexException::RegexException(
	std::string_view _regex, std::string_view _message )
	: m_regex{ _regex }
	, m_message{ _message }
{
}

//------------------------------------------------------------------------------

const char * RegexException::what() const noexcept
{
	if( m_msg.empty() )
	{
		m_msg = m_message + " for pattern: \"" + m_regex + '"';
	}

	return m_msg.c_str();
}

//------------------------------------------------------------------------------

}
