#include "reporter/exceptions/rp_incorrect_report_impl.hpp"

#include "reporter/resources/rp_resources_exceptions.hpp"
#include "reporter/resources/rp_most_impact_report_resources.hpp"
#include "reporter/resources/rp_unresolved_report_resources.hpp"
#include "reporter/resources/rp_unincluded_report_resources.hpp"
#include "reporter/resources/rp_different_type_report_resources.hpp"
#include "reporter/resources/rp_dump_resources.hpp"

#include "reporter/api/enums/rp_reporter_kind.hpp"

#include <fmt/format.h>

#include <string>
#include <string_view>

//------------------------------------------------------------------------------

namespace reporter {

//------------------------------------------------------------------------------

IncorrectReportImpl::IncorrectReportImpl( std::string_view _name )
	:	BaseClass( resources::exceptions::IncorrectReport::Code )
	,	m_name( _name )
{
}

//------------------------------------------------------------------------------

std::string IncorrectReportImpl::getMessage() const noexcept
{
	using namespace resources;

	return fmt::format(
		exceptions::IncorrectReport::MsgFmt,
		m_name,
		getPossiblesValues()
	);
}

//------------------------------------------------------------------------------

std::string IncorrectReportImpl::getPossiblesValues()
{
	constexpr int ReporterKindCount = 5;
	static_assert( static_cast< int >( ReporterKind::Count ) == ReporterKindCount );

	static const std::string seperator = ", ";

	static const std::string result =
		std::string() +
		resources::most_impact_report::Name +
		seperator +
		resources::unresolved_report::Name +
		seperator +
		resources::unincluded_report::Name +
		seperator +
		resources::dump_reporter::Name +
		seperator +
		resources::different_type_report::Name
	;

	return result;
}

//------------------------------------------------------------------------------

}
