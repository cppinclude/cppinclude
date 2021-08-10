#pragma once

//------------------------------------------------------------------------------

namespace application::resources::arguments
{
//------------------------------------------------------------------------------

namespace projectDir
{
extern const char * const FullName;
extern const char * const Description;
extern const char * const DefaultValue;
}

//------------------------------------------------------------------------------

namespace fileExtensions
{
extern const char * const FullName;
extern const char * const Description;
extern const char * const DefaultValues[];
}

//------------------------------------------------------------------------------

namespace analyzeWithoutExtension
{
extern const char * const FullName;
extern const char * const Description;
extern const bool DefaultValue;
}

//------------------------------------------------------------------------------

namespace includeDirs
{
extern const char * const FullName;
extern const char * const Description;
extern const char * const DefaultValues[];
}

//------------------------------------------------------------------------------

namespace ignoreDirs
{
extern const char * const FullName;
extern const char * const Description;
extern const char * const DefaultValues[];
}

//------------------------------------------------------------------------------

namespace ignoreSystemIncludes
{
extern const char * const FullName;
extern const char * const Description;
extern const bool DefaultValue;
}

//------------------------------------------------------------------------------

namespace ignoreFiles
{
extern const char * const FullName;
extern const char * const Description;
extern const char * const DefaultValues[];
}

//------------------------------------------------------------------------------

namespace configurationFile
{
extern const char * const FullName;
extern const char * const Description;
extern const char * const DefaultValue;
}

//------------------------------------------------------------------------------

namespace compileCommands
{
extern const char * const FullName;
extern const char * const Description;
extern const char * const DefaultValue;
}

//------------------------------------------------------------------------------

namespace help
{
extern const char * const FullName;
extern const char * const Description;
}

//------------------------------------------------------------------------------

namespace verbose
{
extern const char * const FullName;
extern const char * const Description;
}

//------------------------------------------------------------------------------

namespace verboseIgnore
{
extern const char * const FullName;
extern const char * const Description;
}

//------------------------------------------------------------------------------

namespace version
{
extern const char * const FullName;
extern const char * const Description;
}

//------------------------------------------------------------------------------

namespace report
{
extern const char * const FullName;
extern const char * const Description;
extern const char * const DefaultValue[];

}

//------------------------------------------------------------------------------

namespace reportLimit
{
extern const char * const FullName;
extern const char * const Description;
extern const int DefaultValue;
}

//------------------------------------------------------------------------------

namespace reportDetailsLimit
{
extern const char * const FullName;
extern const char * const Description;
extern const int DefaultValue;
}

//------------------------------------------------------------------------------

namespace showStdFiles
{
extern const char * const FullName;
extern const char * const Description;
extern const bool DefaultValue;
}

//------------------------------------------------------------------------------

namespace showOnlyStdHeaders
{
extern const char * const FullName;
extern const char * const Description;
extern const bool DefaultValue;
}

//------------------------------------------------------------------------------

namespace showDetails
{
extern const char * const FullName;
extern const char * const Description;
extern const bool DefaultValue;
}

//------------------------------------------------------------------------------

namespace thousandsSeparator
{
extern const char * const FullName;
extern const char * const Description;
extern const char * const DefaultValue;
}

//------------------------------------------------------------------------------

}
