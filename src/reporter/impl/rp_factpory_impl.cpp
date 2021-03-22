#include "reporter/impl/rp_factpory_impl.hpp"

#include "reporter/api/enums/rp_reporter_kind.hpp"

#include "reporter/impl/different_type_reporter/rp_different_type_reporter.hpp"
#include "reporter/impl/dump_reporter/rp_dump_reporter.hpp"
#include "reporter/impl/most_impact_reporter/rp_most_impact_reporter.hpp"
#include "reporter/impl/unincluded_reporter/rp_unincluded_reporter.hpp"
#include "reporter/impl/unresolved_reporter/rp_unresolved_reporter.hpp"

#include "reporter/impl/rp_settings_impl.hpp"

#include "exception/ih/exc_internal_error.hpp"

#include <memory>

//------------------------------------------------------------------------------

namespace reporter {

//------------------------------------------------------------------------------

FactoryImpl::ReporterPtr FactoryImpl::createReporter( ReporterKind _kind )
{
	constexpr int ReporterKindCount = 5;
	static_assert( static_cast< int >( ReporterKind::Count ) == ReporterKindCount );
	switch( _kind )
	{
		case ReporterKind::Unresolved		: return createUnresolvedReporter();
		case ReporterKind::MostImpact		: return createMostImpactReporter();
		case ReporterKind::Unincluded		: return createUnincludedReporter();
		case ReporterKind::Dump				: return createDumpReporter();
		case ReporterKind::DifferentType	: return createDifferentTypeReporter();
		case ReporterKind::Count		:
		{
			INTERNAL_CHECK_WARRING( false );
			return nullptr;
		}
	}

	return nullptr;
}

//------------------------------------------------------------------------------

FactoryImpl::ReporterPtr FactoryImpl::createDumpReporter()
{
	return ReporterPtr{ new DumpReporter{ createSettings() } };
}

//------------------------------------------------------------------------------

FactoryImpl::ReporterPtr FactoryImpl::createUnresolvedReporter()
{
	return ReporterPtr{ new UnresolvedReporter{ createSettings() } };
}

//------------------------------------------------------------------------------

FactoryImpl::ReporterPtr FactoryImpl::createMostImpactReporter()
{
	return ReporterPtr{ new MostImpcatReporter{ createSettings() } };
}

//------------------------------------------------------------------------------

FactoryImpl::ReporterPtr FactoryImpl::createUnincludedReporter()
{
	return ReporterPtr{ new UnincludedReporter{ createSettings() } };
}

//------------------------------------------------------------------------------

FactoryImpl::ReporterPtr FactoryImpl::createDifferentTypeReporter()
{
	return ReporterPtr{ new DifferentTypeReporter{ createSettings() } };
}

//------------------------------------------------------------------------------

FactoryImpl::SettingsPtr FactoryImpl::createSettings()
{
	return SettingsPtr{ new SettingsImpl };
}

//------------------------------------------------------------------------------

}
