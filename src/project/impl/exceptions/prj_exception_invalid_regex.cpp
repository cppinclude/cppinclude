#include "project/impl/exceptions/prj_exception_invalid_regex.hpp"

#include "project/resources/prj_resources_exceptions.hpp"

//------------------------------------------------------------------------------

namespace project {

//------------------------------------------------------------------------------

InvalidRegexImpl::InvalidRegexImpl(
	std::string _filter,
	const std::exception & _originException
)
	:	BaseClass{ resources::exceptions::InvalidRegex::Code }
	,	m_filter{ std::move( _filter ) }
	,	m_originMsg{ _originException.what() }
{
}

//------------------------------------------------------------------------------

std::string InvalidRegexImpl::getMessage() const noexcept
{
	using namespace resources;
	return exceptions::InvalidRegex::Msg + m_filter + " " + m_originMsg;
}

//------------------------------------------------------------------------------

}
