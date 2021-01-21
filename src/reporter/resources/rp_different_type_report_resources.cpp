#include "reporter/resources/rp_different_type_report_resources.hpp"

//------------------------------------------------------------------------------

namespace reporter::resources::different_type_report {

//------------------------------------------------------------------------------

	const char * const Name = "different_type";

	const char * const Header =
		"Files that are included by different ways:\n";
	const char * const UserIncludeLabel =
		"With double quotation marks ( #include \"...\" ) in files:\n";
	const char * const SystemIncludeLabel =
		"With angle brackets ( #include <...> ) in files:\n";

	const char * const FileFmt		= "{}. {}\n";
	const char * const DetailFmt	= "\t{}. {} line {}\n";

//------------------------------------------------------------------------------

}
