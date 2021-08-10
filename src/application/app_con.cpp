#include "application/app_con.hpp"

#include "application/resources/app_resources_messages.hpp"
#include "application/resources/app_resources_version.hpp"
#include "application/tools/app_cmake_project_builder.hpp"
#include "application/tools/app_configuration_file.hpp"
#include "application/tools/app_configuration_file_loader.hpp"
#include "application/tools/app_log.hpp"
#include "application/tools/app_parser_arg_wrapper.hpp"
#include "application/tools/app_project_builder.hpp"
#include "application/tools/app_report_settings_loader.hpp"

#include "application/exceptions/app_cant_create_analyzer.hpp"
#include "application/exceptions/app_cant_create_model_impl.hpp"
#include "application/exceptions/app_cant_create_project_impl.hpp"
#include "application/exceptions/app_cant_create_report_impl.hpp"
#include "application/exceptions/app_cant_load_reporter_settings.hpp"

#include "project/api/prj_project.hpp"
#include "project/ih/prj_project_accesso_impl.hpp"

#include "model_includes/api/mi_analyzer.hpp"
#include "model_includes/api/mi_model.hpp"
#include "model_includes/ih/mi_accessor_impl.hpp"

#include "fs/api/fs_factory.hpp"
#include "fs/ih/fs_accessor_impl.hpp"

#include "parser/api/pr_parser.hpp"
#include "parser/ih/pr_accessor_impl.hpp"

#include "reporter/api/rp_factory.hpp"
#include "reporter/api/rp_reporter.hpp"
#include "reporter/api/rp_settings.hpp"
#include "reporter/ih/rp_accessor_impl.hpp"

#include "json/ih/json_accessor_impl.hpp"

#include "cmake_project/api/cprj_project.hpp"
#include "cmake_project/ih/cprj_accessor_impl.hpp"

#include "compilation_db/ih/cdb_accessor_impl.hpp"

#include "tools/numeric_punct_settings.hpp"

#include <fmt/format.h>

#include <fstream>
#include <functional>
#include <iostream>
#include <std_fs>
#include <vector>

//------------------------------------------------------------------------------

namespace application
{
//------------------------------------------------------------------------------

ConcoleApplication::ConcoleApplication() = default;
ConcoleApplication::~ConcoleApplication() = default;

//------------------------------------------------------------------------------

// NOLINTNEXTLINE(hicpp-avoid-c-arrays, modernize-avoid-c-arrays,
// cppcoreguidelines-avoid-c-arrays)
int ConcoleApplication::run( int _argc, char * _argv[] )
{
	ParserArgWrapper arguments;
	arguments.init();
	arguments.parse( _argc, _argv );

	if( arguments.isHelp() )
	{
		arguments.showHelp( std::cout );
		return EXIT_SUCCESS;
	}
	if( arguments.isVersion() )
	{
		showVersion();
		return EXIT_SUCCESS;
	}

	auto configurationFilePtr = loadConfigurationFile( arguments );

	ProjectPtr projectPtr =
		createProject( arguments, configurationFilePtr.get() );
	if( !projectPtr )
	{
		throw CantCreateProjectImpl{};
	}

	Project & project = *projectPtr;
	if( arguments.isVerbose() )
	{
		dump( project );
	}

	if( arguments.isVerboseIgnore() )
	{
		project.setVerboseIgnore( true );
	}

	CMakeProjectPtr cmakeProjectPtr =
		createCMakeProject( arguments, configurationFilePtr.get() );

	if( arguments.isVerbose() && cmakeProjectPtr )
	{
		dump( *cmakeProjectPtr );
	}

	ModelPtr modelPtr = runAnalyzer( project, cmakeProjectPtr.get() );
	if( !modelPtr )
	{
		throw CantCreateModelImpl{};
	}
	Model & model = *modelPtr;

	ReportSettingsLoader reportSettingsLoader{ getReporterFactory() };
	auto reports = reportSettingsLoader.loadReports(
		arguments, configurationFilePtr.get() );
	auto settingsPtr =
		reportSettingsLoader.load( arguments, configurationFilePtr.get() );

	if( !settingsPtr )
	{
		throw CantLoadReporterSettingsImpl{};
	}

	setSystemSeparators( settingsPtr->getThousandsSeparator() );

	runReporters( model, reports, *settingsPtr );

	return EXIT_SUCCESS;
}

//------------------------------------------------------------------------------

ConcoleApplication::ProjectPtr ConcoleApplication::createProject(
	const ParserArgWrapper & _arguments,
	const ConfigurationFile * _configurationFile )
{
	ProjectBuilder projectBuilder(
		ensureProjectAccessor(), ensureFileSystem() );
	return projectBuilder.build( _arguments, _configurationFile );
}

//------------------------------------------------------------------------------

ConcoleApplication::CMakeProjectPtr ConcoleApplication::createCMakeProject(
	const ParserArgWrapper & _arguments,
	const ConfigurationFile * _configurationFile )
{
	CMakeProjectBuilder projectBuilder{
		ensureCMakeAccessor(), ensureCompilationDbAccessor(),
		ensureJsonAccessor(), ensureFileSystem() };
	return projectBuilder.build( _arguments, _configurationFile );
}

//------------------------------------------------------------------------------

ConcoleApplication::ConfigurationFilePtr
ConcoleApplication::loadConfigurationFile( const ParserArgWrapper & _arguments )
{
	getLog().printLine( resources::messages::StartConfigurateProject );

	ConfigurationFileLoader loader( ensureJsonAccessor(), ensureFileSystem() );
	return loader.load( _arguments );
}

//------------------------------------------------------------------------------

ConcoleApplication::ModelPtr ConcoleApplication::runAnalyzer(
	const Project & _project, const CMakeProject * _cmakeProject )
{
	getLog().printLine( resources::messages::StartAnalyzeSources );

	AnalyzerPtr analyzerPtr = createAnalyzer();
	if( !analyzerPtr )
	{
		throw CantCreateAnalyzerImpl();
		return nullptr;
	}

	ModelPtr result;
	if( _cmakeProject != nullptr )
	{
		result = analyzerPtr->analyze( _project, *_cmakeProject );
	}
	else
	{
		result = analyzerPtr->analyze( _project );
	}
	return result;
}

//------------------------------------------------------------------------------

void ConcoleApplication::runReporters(
	const Model & _model,
	const ReporterKinds & _kinds,
	const reporter::Settings & _reporterSettings )
{
	using namespace reporter;

	getLog().printLine( resources::messages::StartPrintResult );

	Factory & factory = ensureReporterAccessor().getReporterFactory();

	for( reporter::ReporterKind kind: _kinds )
	{
		auto reportPtr = factory.createReporter( kind );
		if( !reportPtr )
		{
			getLog().printWarning( CantCreateReportImpl{ kind } );
			continue;
		}

		Report & report = *reportPtr;
		report.copySettings( _reporterSettings );
		report.report( _model, std::cout );
	}
}

//------------------------------------------------------------------------------

ConcoleApplication::ProjectAccessor &
ConcoleApplication::ensureProjectAccessor()
{
	return m_projectAccessor.ensure< project::ProjectAccessorImpl >();
}

//------------------------------------------------------------------------------

ConcoleApplication::ModelIncludesAccessor &
ConcoleApplication::ensureModelIncludesAccessor()
{
	using namespace model_includes;

	return m_modelIncludesAccessor.ensure< ModelIncludesAccessorImpl >();
}

//------------------------------------------------------------------------------

ConcoleApplication::AnalyzerPtr ConcoleApplication::createAnalyzer()
{
	fs::FileSystem & fs = ensureFileSystem();
	const parser::Parser & parser = ensureParser();
	auto & accessor = ensureModelIncludesAccessor();
	return accessor.createAnalyzer( fs, parser );
}

//------------------------------------------------------------------------------

fs::FileSystemAccessor & ConcoleApplication::ensureFileSystemAccessor()
{
	return m_fileSystemAccessor.ensure< fs::FileSystemAccessorImpl >();
}

//------------------------------------------------------------------------------

fs::FileSystem & ConcoleApplication::ensureFileSystem()
{
	return ensureFileSystemAccessor().getFactory().getPhysicalFileSystem();
}

//------------------------------------------------------------------------------

ConcoleApplication::ParserAccessor & ConcoleApplication::ensureParserAccessor()
{
	return m_parserAccessor.ensure< parser::ParserAccessorImpl >();
}

//------------------------------------------------------------------------------

const ConcoleApplication::Parser & ConcoleApplication::ensureParser()
{
	if( !m_parser )
	{
		auto newParser = ensureParserAccessor().createParser();
		m_parser.swap( newParser );
	}

	return *m_parser;
}

//------------------------------------------------------------------------------

ConcoleApplication::ReporterAccessor &
ConcoleApplication::ensureReporterAccessor()
{
	return m_reporterAccessor.ensure< reporter::ReporterAccessorImpl >();
}

//------------------------------------------------------------------------------

reporter::Factory & ConcoleApplication::getReporterFactory()
{
	return ensureReporterAccessor().getReporterFactory();
}

//------------------------------------------------------------------------------

ConcoleApplication::JsonAccessor & ConcoleApplication::ensureJsonAccessor()
{
	return m_jsonAccessor.ensure< json::JsonAccesorImpl >();
}

//------------------------------------------------------------------------------

ConcoleApplication::CMakeAccessor & ConcoleApplication::ensureCMakeAccessor()
{
	return m_cmakeAccessor.ensure< cmake_project::AccessorImpl >();
}

//------------------------------------------------------------------------------

ConcoleApplication::CompilationDbAccessor &
ConcoleApplication::ensureCompilationDbAccessor()
{
	return m_compilationDbAccessor.ensure< compilation_db::AccessorImpl >();
}

//------------------------------------------------------------------------------

Log & ConcoleApplication::getLog()
{
	if( !m_log )
	{
		m_log = std::make_unique< Log >( std::cout, std::cerr );
	}
	return *m_log;
}

//------------------------------------------------------------------------------

void ConcoleApplication::dump( const Project & _project ) const
{
	std::cout << "project directory : " << _project.getProjectDir()
			  << std::endl;

	const Project::IncludeDirIndex includeDirsCount =
		_project.getIncludeDirsCount();
	if( includeDirsCount > 0 )
	{
		std::cout << "Include dirs:\n";
		for( Project::IncludeDirIndex i = 0; i < includeDirsCount; ++i )
		{
			std::cout << "\t" << i + 1 << " : " << _project.getIncludeDir( i )
					  << std::endl;
		}
	}
	int ignoreDirNumber = 1;
	_project.forEachIgnoreDir( [&]( const stdfs::path & _path ) {
		if( ignoreDirNumber == 1 )
		{
			std::cout << "Ignore dirs:\n";
		}

		std::cout << "\t" << ignoreDirNumber << " : " << _path << std::endl;
		++ignoreDirNumber;
		return true;
	} );

	int fileExtensionNumber = 1;
	_project.forEachFileExtension( [&]( std::string_view _ext ) {
		if( fileExtensionNumber == 1 )
		{
			std::cout << "File extensions\n";
		}

		std::cout << "\t" << fileExtensionNumber << " : " << _ext << std::endl;

		++fileExtensionNumber;
		return true;
	} );
}

//------------------------------------------------------------------------------

void ConcoleApplication::dump( const CMakeProject & _project ) const
{
	std::cout << "cmake project dump:" << std::endl;
	int fileNumber = 1;
	_project.forEachFilePath( [&]( const CMakeProject::Path & _path ) {
		std::cout << fileNumber << " : " << _path << std::endl;
		std::cout << "includes:" << std::endl;
		int includeNumber = 1;
		_project.forEachIncludes(
			_path, [&]( const CMakeProject::Path & _include ) {
				std::cout << '\t' << includeNumber << " : " << _include
						  << std::endl;
				++includeNumber;
				return true;
			} );

		++fileNumber;
		return true;
	} );
}

//------------------------------------------------------------------------------

void ConcoleApplication::showVersion()
{
	using namespace resources;

	std::cout << fmt::format(
		version::VersionFormat, version::Major, version::Minor,
		version::Patch );
}

//------------------------------------------------------------------------------

void ConcoleApplication::setSystemSeparators( char _separator )
{
	tools::NumericPunctSettings::setSystemSeparators( _separator );
}

//------------------------------------------------------------------------------

}
