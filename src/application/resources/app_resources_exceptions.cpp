#include "application/resources/app_resources_exceptions.hpp"

//------------------------------------------------------------------------------

namespace application::resources::exceptions {

//------------------------------------------------------------------------------

	const char * const ModuleName = "APP";

//------------------------------------------------------------------------------

namespace CantCreateProject {

	const char * const Code			= "CREATE_PROJECT";
	const char * const Msg			= "Can't create project";
}

//------------------------------------------------------------------------------

namespace CantCreateModel {

	const char * const Code			= "CREATE_MODEL";
	const char * const Msg			= "Can't create model";
}

//------------------------------------------------------------------------------

namespace CantCreateAnalyzer {

	const char * const Code			= "CREATE_ANALYZER";
	const char * const Msg			= "Can't create analyzer";
}

//------------------------------------------------------------------------------

namespace CantCreateReport {

	const char * const Code			= "CREATE_REPORT";
	const char * const Msg			= "Can't create report ";
}

//------------------------------------------------------------------------------

namespace CantLoadReporterSettings {

	const char * const Code			= "LOAD_REPORTER_SETTINGS";
	const char * const Msg			= "Can't load settings for reporters";
}

//------------------------------------------------------------------------------

}
