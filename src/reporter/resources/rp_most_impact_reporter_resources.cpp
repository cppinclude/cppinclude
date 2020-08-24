#include "reporter/resources/rp_most_impact_reporter_resources.hpp"

//------------------------------------------------------------------------------

namespace reporter::resources::most_impact_reporter {

//------------------------------------------------------------------------------

	const char * const Header						= "Most impact files:\n";

	const char * const LineForFileFmt				= "{} : \"{}\" impact to {} file(s)\n";
	const char * const HeaderForDetails				= "Included by:\n";
	const char * const LineForDetailFmt				= "\t{} : \"{}\" line {}, impact to {} file(s)\n";
	const char * const LineForNotImpactDetailFmt	= "\t{} : \"{}\" line {}\n";

//------------------------------------------------------------------------------

}
