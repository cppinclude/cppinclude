#include "reporter/resources/rp_unresolved_report_resources.hpp"

//------------------------------------------------------------------------------

namespace reporter::resources::unresolved_report
{
//------------------------------------------------------------------------------

const char * const Name = "unresolved";

const char * const Header = "Unresolved file{}:\n";
const char * const Intend = "\t";

const char * const UnresolvedDestinationFmt =
	"{:L} : \"{}\" isn't resolved in:\n";
const char * const UnresolvedDestinationWithoutDetailsFmt =
	"{:L} : \"{}\" isn't resolved\n";
const char * const UnresolvedSourceFmt = "{:L} : \"{}\" line: {:L}\n";

//------------------------------------------------------------------------------

}
