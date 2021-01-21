#pragma once

//------------------------------------------------------------------------------

namespace application::resources::arguments {

//------------------------------------------------------------------------------

namespace projectDir {

	extern const char * const FullName;
	extern const char * const Description;
	extern const char * const DefaultValue;
}

//------------------------------------------------------------------------------

namespace fileExtensions {

	extern const char * const FullName;
	extern const char * const Description;
	extern const char * const DefaultValue[];
}

//------------------------------------------------------------------------------

namespace analyze_without_extension {

	extern const char * const FullName;
	extern const char * const Description;
	extern const bool DefaultValue;
}

//------------------------------------------------------------------------------

namespace includeDirs {

	extern const char * const FullName;
	extern const char * const Description;
	extern const char * const DefaultValue[];
}

//------------------------------------------------------------------------------

namespace ignoreDirs {

	extern const char * const FullName;
	extern const char * const Description;
	extern const char * const DefaultValue[];
}

//------------------------------------------------------------------------------

namespace ignoreSystemIncludes {

	extern const char * const FullName;
	extern const char * const Description;
	extern const bool DefaultValue;
}

//------------------------------------------------------------------------------

namespace ignoreFiles {

	extern const char * const FullName;
	extern const char * const Description;
	extern const char * const DefaultValue[];
}

//------------------------------------------------------------------------------

namespace configurationFile {

	extern const char * const FullName;
	extern const char * const Description;
	extern const char * const DefaultValue;
}

//------------------------------------------------------------------------------

namespace compileCommands {

	extern const char * const FullName;
	extern const char * const Description;
	extern const char * const DefaultValue;
}

//------------------------------------------------------------------------------

namespace help {

	extern const char * const FullName;
	extern const char * const Description;
}

//------------------------------------------------------------------------------

namespace verbose {

	extern const char * const FullName;
	extern const char * const Description;
}

//------------------------------------------------------------------------------

namespace version {

	extern const char * const FullName;
	extern const char * const Description;
}

//------------------------------------------------------------------------------

namespace report {

	extern const char * const FullName;
	extern const char * const Description;
	extern const char * const DefaultValue[];

}

//------------------------------------------------------------------------------

namespace report_limit {

	extern const char * const FullName;
	extern const char * const Description;
	extern const int DefaultValue;
}

//------------------------------------------------------------------------------

namespace report_details_limit {

	extern const char * const FullName;
	extern const char * const Description;
	extern const int DefaultValue;
}

//------------------------------------------------------------------------------

namespace show_std_files {

	extern const char * const FullName;
	extern const char * const Description;
	extern const bool DefaultValue;
}

//------------------------------------------------------------------------------
}
