#include "application/resources/app_resources_arguments.hpp"

#include "application/resources/app_resources_configuration_file.hpp"

#include "reporter/resources/rp_most_impact_report_resources.hpp"
#include "reporter/resources/rp_unresolved_report_resources.hpp"
#include "reporter/resources/rp_unincluded_report_resources.hpp"
#include "reporter/resources/rp_dump_resources.hpp"

#include "reporter/api/enums/rp_reporter_kind.hpp"

//------------------------------------------------------------------------------

namespace application::resources::arguments {

//------------------------------------------------------------------------------

namespace projectDir {

	const char * const FullName			= configuration_file::ProjectDir;
	const char * const Description		= "Project directory";
	const char * const DefaultValue		= ".";
}

//------------------------------------------------------------------------------

namespace fileExtensions {

	const char * const FullName			= configuration_file::FileExtensions;
	const char * const Description		= "Extensions C++ files";
	const char * const DefaultValue[]	= {"*.cpp", "*.hpp", "*.c", "*.h", "*.cxx", "*.hxx", nullptr};
}

//------------------------------------------------------------------------------

namespace analyze_without_extension {

	const char * const FullName			= configuration_file::AnalyzeWithoutExtension;
	const char * const Description		= "Analyze files without extension (default: false)";
	const bool DefaultValue				= false;
}

//------------------------------------------------------------------------------

namespace includeDirs {

	const char * const FullName			= configuration_file::IncludeDirs ;
	const char * const Description		= "Include directories";
	const char * const DefaultValue[]	= {".", nullptr};
}

//------------------------------------------------------------------------------

namespace ignoreDirs {

	const char * const FullName			= configuration_file::IgnoreDirs;
	const char * const Description		= "Directories that will be ignored";
	const char * const DefaultValue[]	= {nullptr};
}

//------------------------------------------------------------------------------

namespace ignoreSystemIncludes {

	const char * const FullName			= configuration_file::IgnoreSystemIncludes;
	const char * const Description		= "Ignore headers in <> (default: false)";
	const bool DefaultValue				= false;
}

//------------------------------------------------------------------------------

namespace ignoreFiles {

	const char * const FullName			= configuration_file::IgnoreFiles;
	const char * const Description		= "Files will be ignored by regexp";
	const char * const DefaultValue[]	= {nullptr};
}

//------------------------------------------------------------------------------

namespace configurationFile {

	const char * const FullName			= "configuration_file";
	const char * const Description		= "Path to configuration file";
	const char * const DefaultValue		= ".cppinclude.json";
}

//------------------------------------------------------------------------------

namespace compileCommands {

	const char * const FullName			= configuration_file::CompileCommands;
	const char * const Description		= "Path to JSON Compilation Database";
	const char * const DefaultValue		= "compile_commands.json";
}

//------------------------------------------------------------------------------

namespace help {

	const char * const FullName			= "help";
	const char * const Description		= "Show usage";
}

//------------------------------------------------------------------------------

namespace verbose {

	const char * const FullName			= "verbose";
	const char * const Description		= "Verbose mode";
}

//------------------------------------------------------------------------------

namespace version {

	const char * const FullName			= "version";
	const char * const Description		= "Show application version";
}

//------------------------------------------------------------------------------

namespace report {

	using namespace reporter::resources;

	const char * const FullName			= configuration_file::Report;

	static_assert( static_cast< int >( reporter::ReporterKind::Count ) == 5 );
	const char * const Description		=
		"List of reports. Name of reports: "
		"unresolved, "
		"most_impact, "
		"unincluded, "
		"different_type"
		;

	const char * const DefaultValue[]	= {
		unresolved_report::Name,
		most_impact_report::Name,
		unincluded_report::Name,

		nullptr
	};
}

//------------------------------------------------------------------------------

namespace report_limit {

	const char * const FullName			= configuration_file::ReportLimit;
	const char * const Description		= "Maximum elements in report, 0 - unlimited";
	const int DefaultValue				= 10;
}

//------------------------------------------------------------------------------

namespace report_details_limit {

	const char * const FullName			= configuration_file::ReportDetailsLimit;
	const char * const Description		= "Maximum details in report, 0 - unlimited";
	const int DefaultValue				= 10;
}

//------------------------------------------------------------------------------

namespace show_std_files {

	const char * const FullName			= configuration_file::ShowStdFiles;
	const char * const Description		= "Show standard library headers in output (default: false)";
	const bool DefaultValue				= false;
}

//------------------------------------------------------------------------------

}
