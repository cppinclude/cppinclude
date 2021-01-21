#include "reporter/resources/rp_most_impact_report_resources.hpp"

//------------------------------------------------------------------------------

namespace reporter::resources::most_impact_report {

//------------------------------------------------------------------------------

	const char * const Name							= "most_impact";

	const char * const Header						= "Most impact files:\n";

	const char * const LineForFileFmt				= "{} : \"{}\" impact on {} file(s)\n";
	const char * const HeaderForDetails				= "Included by:\n";
	const char * const LineForDetailFmt				= "\t{} : \"{}\" line {}, impact on {} file(s)\n";
	const char * const LineForNotImpactDetailFmt	= "\t{} : \"{}\" line {}\n";

//------------------------------------------------------------------------------

}
