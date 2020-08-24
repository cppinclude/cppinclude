#include "reporter/impl/rp_factpory_impl.hpp"

#include "reporter/api/enums/rp_reporter_kind.hpp"

#include "reporter/impl/rp_dump_reporter.hpp"
#include "reporter/impl/rp_unresolved_reporter.hpp"
#include "reporter/impl/rp_most_impact_reporter.hpp"

#include "exception/ih/exc_internal_error.hpp"

#include <memory>

//------------------------------------------------------------------------------

namespace reporter {

//------------------------------------------------------------------------------

FactoryImpl::ReporterPtr FactoryImpl::createReporter( ReporterKind _kind )
{
	static_assert( static_cast< int >( ReporterKind::Count ) == 3 );
	switch( _kind )
	{
		case ReporterKind::Unresolved	: return createUnresolvedReporter();
		case ReporterKind::MostImpact	: return createMostImpactReporter();
		case ReporterKind::Dump			: return createDumpReporter();
		case ReporterKind::Count		:
		{
			INTERNAL_CHECK_WARRING(false);
			return nullptr;
		}
	}

	return nullptr;
}

//------------------------------------------------------------------------------

FactoryImpl::ReporterPtr FactoryImpl::createDumpReporter()
{
	return ReporterPtr{ new DumpReporter };
}

//------------------------------------------------------------------------------

FactoryImpl::ReporterPtr FactoryImpl::createUnresolvedReporter()
{
	return ReporterPtr{ new UnresolvedReporter };
}

//------------------------------------------------------------------------------

FactoryImpl::ReporterPtr FactoryImpl::createMostImpactReporter()
{
	return ReporterPtr{ new MostImpcatReporter };
}

//------------------------------------------------------------------------------

}
