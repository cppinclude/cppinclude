#include "project/impl/exceptions/prj_exception_invalid_regex.hpp"

#include "project/resources/prj_resources_exceptions.hpp"

//------------------------------------------------------------------------------

namespace project
{
//------------------------------------------------------------------------------

InvalidRegexImpl::InvalidRegexImpl( const std::exception & _originException )
	: BaseClass{ resources::exceptions::InvalidRegex::Code }
	, m_originMsg{ _originException.what() }
{
}

//------------------------------------------------------------------------------

std::string InvalidRegexImpl::getMessage() const noexcept
{
	return resources::exceptions::InvalidRegex::Msg + m_originMsg;
}

//------------------------------------------------------------------------------

}
