#include "application/resources/app_resources_arguments.hpp"

#include "application/resources/app_resources_configuration_file.hpp"

#include "reporter/resources/rp_dump_resources.hpp"
#include "reporter/resources/rp_most_impact_report_resources.hpp"
#include "reporter/resources/rp_unincluded_report_resources.hpp"
#include "reporter/resources/rp_unresolved_report_resources.hpp"

#include "reporter/api/enums/rp_reporter_kind.hpp"

//------------------------------------------------------------------------------

namespace application::resources::arguments
{
//------------------------------------------------------------------------------

namespace projectDir
{
// NOLINTNEXTLINE(cppcoreguidelines-interfaces-global-init)
const char * const FullName = resources::configurationFile::ProjectDir;
const char * const Description = "Project directory";
const char * const DefaultValue = ".";
}

//------------------------------------------------------------------------------

namespace fileExtensions
{
// NOLINTNEXTLINE(cppcoreguidelines-interfaces-global-init)
const char * const FullName = resources::configurationFile::FileExtensions;
const char * const Description = "Extensions C++ files";
// NOLINTNEXTLINE(hicpp-avoid-c-arrays, modernize-avoid-c-arrays,
// cppcoreguidelines-avoid-c-arrays)
const char * const DefaultValues[] = { "*.cpp", "*.hpp", "*.c",	 "*.h",
									   "*.cxx", "*.hxx", nullptr };
}

//------------------------------------------------------------------------------

namespace analyzeWithoutExtension
{
// NOLINTNEXTLINE(cppcoreguidelines-interfaces-global-init)
const char * const FullName =
	resources::configurationFile::AnalyzeWithoutExtension;
const char * const Description =
	"Analyze files without extension (default: false)";
const bool DefaultValue = false;
}

//------------------------------------------------------------------------------

namespace includeDirs
{
// NOLINTNEXTLINE(cppcoreguidelines-interfaces-global-init)
const char * const FullName = resources::configurationFile::IncludeDirs;
const char * const Description = "Include directories";
// NOLINTNEXTLINE(hicpp-avoid-c-arrays, modernize-avoid-c-arrays,
// cppcoreguidelines-avoid-c-arrays)
const char * const DefaultValues[] = { ".", nullptr };
}

//------------------------------------------------------------------------------

namespace ignoreDirs
{
// NOLINTNEXTLINE(cppcoreguidelines-interfaces-global-init)
const char * const FullName = resources::configurationFile::IgnoreDirs;
const char * const Description = "Directories that will be ignored";
// NOLINTNEXTLINE(hicpp-avoid-c-arrays, modernize-avoid-c-arrays,
// cppcoreguidelines-avoid-c-arrays)
const char * const DefaultValues[] = { nullptr };
}

//------------------------------------------------------------------------------

namespace ignoreSystemIncludes
{
// NOLINTNEXTLINE(cppcoreguidelines-interfaces-global-init)
const char * const FullName =
	resources::configurationFile::IgnoreSystemIncludes;
const char * const Description = "Ignore headers in <> (default: false)";
const bool DefaultValue = false;
}

//------------------------------------------------------------------------------

namespace ignoreFiles
{
// NOLINTNEXTLINE(cppcoreguidelines-interfaces-global-init)
const char * const FullName = resources::configurationFile::IgnoreFiles;
const char * const Description = "Files will be ignored by regexp";
// NOLINTNEXTLINE(hicpp-avoid-c-arrays, modernize-avoid-c-arrays,
// cppcoreguidelines-avoid-c-arrays)
const char * const DefaultValues[] = { nullptr };
}

//------------------------------------------------------------------------------

namespace configurationFile
{
const char * const FullName = "configuration_file";
const char * const Description = "Path to configuration file";
const char * const DefaultValue = ".cppinclude.json";
}

//------------------------------------------------------------------------------

namespace compileCommands
{
// NOLINTNEXTLINE(cppcoreguidelines-interfaces-global-init)
const char * const FullName = resources::configurationFile::CompileCommands;
const char * const Description = "Path to JSON Compilation Database";
const char * const DefaultValue = "compile_commands.json";
}

//------------------------------------------------------------------------------

namespace help
{
const char * const FullName = "help";
const char * const Description = "Show usage";
}

//------------------------------------------------------------------------------

namespace verbose
{
const char * const FullName = "verbose";
const char * const Description = "Verbose mode";
}

//------------------------------------------------------------------------------

namespace version
{
const char * const FullName = "version";
const char * const Description = "Show application version";
}

//------------------------------------------------------------------------------

namespace verboseIgnore
{
const char * const FullName = "verbose_ignore";
const char * const Description = "Show ignored files and folders";
}

//------------------------------------------------------------------------------

namespace report
{
using namespace reporter::resources;

// NOLINTNEXTLINE(cppcoreguidelines-interfaces-global-init)
const char * const FullName = resources::configurationFile::Report;

constexpr int ReporterKindCount = 5;
static_assert(
	static_cast< int >( reporter::ReporterKind::Count ) == ReporterKindCount );
const char * const Description = "List of reports. Name of reports: "
								 "unresolved, "
								 "most_impact, "
								 "unincluded, "
								 "different_type";

// NOLINTNEXTLINE(hicpp-avoid-c-arrays, modernize-avoid-c-arrays,
// cppcoreguidelines-avoid-c-arrays, cppcoreguidelines-interfaces-global-init)
const char * const DefaultValue[] = {
	unresolved_report::Name, most_impact_report::Name, unincluded_report::Name,

	nullptr };
}

//------------------------------------------------------------------------------

namespace reportLimit
{
// NOLINTNEXTLINE(cppcoreguidelines-interfaces-global-init)
const char * const FullName = resources::configurationFile::ReportLimit;
const char * const Description = "Maximum elements in report, 0 - unlimited";
const int DefaultValue = 10;
}

//------------------------------------------------------------------------------

namespace reportDetailsLimit
{
// NOLINTNEXTLINE(cppcoreguidelines-interfaces-global-init)
const char * const FullName = resources::configurationFile::ReportDetailsLimit;
const char * const Description = "Maximum details in report, 0 - unlimited";
const int DefaultValue = 10;
}

//------------------------------------------------------------------------------

namespace showStdFiles
{
// NOLINTNEXTLINE(cppcoreguidelines-interfaces-global-init)
const char * const FullName = resources::configurationFile::ShowStdFiles;
const char * const Description =
	"Show standard library headers in output (default: false)";
const bool DefaultValue = false;
}

//------------------------------------------------------------------------------

namespace showOnlyStdHeaders
{
// NOLINTNEXTLINE(cppcoreguidelines-interfaces-global-init)
const char * const FullName = resources::configurationFile::ShowOnlyStdHeaders;
const char * const Description =
	"Show only standard library headers in output (default: false)";
const bool DefaultValue = false;
}

//------------------------------------------------------------------------------

namespace showDetails
{
// NOLINTNEXTLINE(cppcoreguidelines-interfaces-global-init)
const char * const FullName = resources::configurationFile::ShowDetails;
const char * const Description = "Show details in output";
const bool DefaultValue = true;
}

//------------------------------------------------------------------------------

namespace thousandsSeparator
{
const char * const FullName = resources::configurationFile::ThousandsSeparator;
const char * const Description =
	"Set thousands separator, for example ','";
const char * const DefaultValue = " ";
}

//------------------------------------------------------------------------------

}
