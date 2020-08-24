#include "fs/resources/fs_resources_exceptions.hpp"

//------------------------------------------------------------------------------

namespace fs::resources::exceptions {

//------------------------------------------------------------------------------

	const char * const ModuleName = "FS";

//------------------------------------------------------------------------------

namespace CantCreateFile
{
	const char * const Code		= "CREATE_FILE";
	const char * const Msg		= "Can't create file: ";
}

//------------------------------------------------------------------------------

namespace CantOpenFile
{
	const char * const Code		= "OPEN_FILE";
	const char * const Msg		= "Can't open file: ";
}

//------------------------------------------------------------------------------

}
