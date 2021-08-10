#include "reporter/resources/rp_most_impact_report_resources.hpp"

//------------------------------------------------------------------------------

namespace reporter::resources::most_impact_report
{
//------------------------------------------------------------------------------

const char * const Name = "most_impact";

const char * const Header = "Most impact file{}:\n";

const char * const LineForFileFmt = "{:L} : \"{}\" impact on {:L} file{}\n";
const char * const HeaderForDetails = "Included by:\n";
const char * const LineForDetailFmt =
	"\t{:L} : \"{}\" line {:L}, impact on {:L} file{}\n";
const char * const LineForNotImpactDetailFmt = "\t{:L} : \"{}\" line {:L}\n";

//------------------------------------------------------------------------------

}
