#pragma once

#include <exception>

#include <string>

//------------------------------------------------------------------------------

namespace tools
{
//------------------------------------------------------------------------------

class RegexException final : public std::exception
{
public:
	RegexException( std::string_view _regex, std::string_view _message );

	const char * what() const noexcept override;

private:
	const std::string m_regex;
	const std::string m_message;

	mutable std::string m_msg;
};

//------------------------------------------------------------------------------

}
