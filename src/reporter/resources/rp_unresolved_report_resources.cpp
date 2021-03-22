#include "reporter/resources/rp_unresolved_report_resources.hpp"

//------------------------------------------------------------------------------

namespace reporter::resources::unresolved_report {

//------------------------------------------------------------------------------

	const char * const Name = "unresolved";

	const char * const Header = "Unresolved files:\n";
	const char * const Intend = "\t";

	const char * const UnresolvedDestinationFmt = "{}. \"{}\" isn't resolved in:\n";
	const char * const UnresolvedSourceFmt = "{}. \"{}\" line: {}\n";

//------------------------------------------------------------------------------

}
