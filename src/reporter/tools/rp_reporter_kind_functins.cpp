#include "reporter/tools/rp_reporter_kind_functins.hpp"

#include "reporter/api/enums/rp_reporter_kind.hpp"

#include "reporter/resources/rp_most_impact_report_resources.hpp"
#include "reporter/resources/rp_unresolved_report_resources.hpp"
#include "reporter/resources/rp_unincluded_report_resources.hpp"
#include "reporter/resources/rp_dump_resources.hpp"
#include "reporter/resources/rp_different_type_report_resources.hpp"

#include "reporter/exceptions/rp_incorrect_report_impl.hpp"

#include "exception/ih/exc_internal_error.hpp"

#include <string_view>
#include <string>
#include <unordered_map>

//------------------------------------------------------------------------------

namespace reporter {

//------------------------------------------------------------------------------

ReporterKind toReporterKind( std::string_view _str )
{
	using namespace resources;

	static_assert( static_cast< int >( ReporterKind::Count ) == 5 );

	static std::unordered_map< std::string, reporter::ReporterKind > names{
		{ unresolved_report::Name,		ReporterKind::Unresolved },
		{ most_impact_report::Name,		ReporterKind::MostImpact },
		{ unincluded_report::Name,		ReporterKind::Unincluded},
		{ dump_reporter::Name,			ReporterKind::Dump },
		{ different_type_report::Name,	ReporterKind::DifferentType },
	};

	const std::string str{ _str };
	if( !names.count( str ) )
	{
		throw IncorrectReportImpl( str );
	}

	return names[ str ];
}

//------------------------------------------------------------------------------

std::string reporterKindToString( ReporterKind _kind )
{
	using namespace resources;

	static_assert( static_cast< int >( ReporterKind::Count ) == 5 );

	switch( _kind )
	{
		case ReporterKind::MostImpact	: return most_impact_report::Name;
		case ReporterKind::Unresolved	: return unresolved_report::Name;
		case ReporterKind::Dump			: return dump_reporter::Name;
		case ReporterKind::Unincluded	: return unincluded_report::Name;
		case ReporterKind::DifferentType: return different_type_report::Name;
		case ReporterKind::Count		:
		{
			INTERNAL_CHECK_WARRING( false );
			return "";
		}
	}
	INTERNAL_CHECK_WARRING( false );
	return "";
}

//------------------------------------------------------------------------------

}
