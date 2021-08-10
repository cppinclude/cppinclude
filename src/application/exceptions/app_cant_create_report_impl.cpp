#include "application/exceptions/app_cant_create_report_impl.hpp"

#include "application/resources/app_resources_arguments.hpp"
#include "application/resources/app_resources_exceptions.hpp"

#include "reporter/tools/rp_reporter_kind_functins.hpp"

#include "exception/ih/exc_internal_error.hpp"

//------------------------------------------------------------------------------

namespace application
{
//------------------------------------------------------------------------------

CantCreateReportImpl::CantCreateReportImpl( reporter::ReporterKind _kind )
	: BaseClass( resources::exceptions::CantCreateReport::Code )
	, m_kind{ _kind }
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
	return reporter::reporterKindToString( _kind );
}

//------------------------------------------------------------------------------

}
