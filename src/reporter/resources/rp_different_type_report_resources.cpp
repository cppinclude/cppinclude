#include "reporter/resources/rp_different_type_report_resources.hpp"

//------------------------------------------------------------------------------

namespace reporter::resources::different_type_report
{
//------------------------------------------------------------------------------

const char * const Name = "different_type";

const char * const Header = "File{} that are included by different ways ( "
							"#include \"...\" and #include <...> ) :\n";
const char * const UserIncludeLabel =
	"With double quotation marks ( #include \"...\" ) in file{}:\n";
const char * const SystemIncludeLabel =
	"With angle brackets ( #include <...> ) in file{}:\n";

const char * const FileFmt = "{:L} : {}\n";
const char * const DetailFmt = "\t{:L} : {} line {:L}\n";

//------------------------------------------------------------------------------

}
