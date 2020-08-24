#include "application/exceptions/app_incorrect_report_impl.hpp"

#include "application/resources/app_resources_exceptions.hpp"
#include "application/resources/app_resources_arguments.hpp"

#include <fmt/format.h>

//------------------------------------------------------------------------------

namespace application {

//------------------------------------------------------------------------------

IncorrectReportImpl::IncorrectReportImpl(
	std::string_view _name,
	const Names & _names
)
	:	BaseClass( resources::exceptions::IncorrectReport::Code )
	,	m_possibleNames( _names )
	,	m_name( _name )
{

}

//------------------------------------------------------------------------------

std::string IncorrectReportImpl::getMessage() const noexcept
{
	return fmt::format(
		resources::exceptions::IncorrectReport::MsgFmt,
		m_name,
		getNames()
	);
}

//------------------------------------------------------------------------------

std::string IncorrectReportImpl::getNames() const
{
	std::string result;
	for( const auto & pair : m_possibleNames )
	{
		if( !result.empty() )
			result += ", ";

		result += pair.first;
	}
	return result;
}

//------------------------------------------------------------------------------

}
