#include "application/exceptions/app_cant_create_report_impl.hpp"

#include "application/resources/app_resources_exceptions.hpp"
#include "application/resources/app_resources_arguments.hpp"

#include "exception/ih/exc_internal_error.hpp"

//------------------------------------------------------------------------------

namespace application {

//------------------------------------------------------------------------------

CantCreateReportImpl::CantCreateReportImpl( reporter::ReporterKind _kind )
	:	BaseClass( resources::exceptions::CantCreateReport::Code )
	,	m_kind{ _kind }
{

}

//------------------------------------------------------------------------------

std::string CantCreateReportImpl::getMessage() const noexcept
{
	return resources::exceptions::CantCreateReport::Msg + toString( m_kind );
}

//------------------------------------------------------------------------------

std::string CantCreateReportImpl::toString( reporter::ReporterKind _kind )
{
	using namespace reporter;
	using namespace resources;

	static_assert( static_cast< int >( ReporterKind::Count ) == 3 );
	switch( _kind )
	{
		case ReporterKind::Dump			: return arguments::report::DumpReport;
		case ReporterKind::MostImpact	: return arguments::report::MostImpactReport;
		case ReporterKind::Unresolved	: return arguments::report::UnresolvedReport;
		default:
			THROW_INTERNAL_ERROR
	}
	THROW_INTERNAL_ERROR
	return "";
}

//------------------------------------------------------------------------------

}
