#include "application/app_con.hpp"

#include "application/tools/app_parser_arg_wrapper.hpp"
#include "application/tools/app_project_builder.hpp"
#include "application/tools/app_log.hpp"
#include "application/resources/app_resources_messages.hpp"
#include "application/resources/app_resources_version.hpp"

#include "application/exceptions/app_cant_create_project_impl.hpp"
#include "application/exceptions/app_cant_create_model_impl.hpp"
#include "application/exceptions/app_cant_create_analyzer.hpp"
#include "application/exceptions/app_cant_create_report_impl.hpp"
#include "application/exceptions/app_cant_create_parser_impl.hpp"

#include "project/ih/prj_project_accesso_impl.hpp"
#include "project/api/prj_project.hpp"

#include "model_includes/ih/mi_accessor_impl.hpp"
#include "model_includes/api/mi_analyzer.hpp"
#include "model_includes/api/mi_model.hpp"

#include "fs/ih/fs_accessor_impl.hpp"
#include "fs/api/fs_factory.hpp"

#include "parser/ih/pr_accessor_impl.hpp"
#include "parser/api/pr_parser.hpp"

#include "reporter/ih/rp_accessor_impl.hpp"
#include "reporter/api/rp_factory.hpp"
#include "reporter/api/rp_reporter.hpp"

#include "json/ih/json_accessor_impl.hpp"

#include <fmt/format.h>

#include <iostream>
#include <vector>
#include <fstream>
#include <filesystem>
#include <functional>

//------------------------------------------------------------------------------

namespace application {

//------------------------------------------------------------------------------

ConcoleApplication::ConcoleApplication() = default;
ConcoleApplication::~ConcoleApplication() = default;

//------------------------------------------------------------------------------

int ConcoleApplication::run( int _argc, char * _argv[] )
{
	ParserArgWrapper parser;
	parser.init();
	parser.parse( _argc, _argv );

	if( parser.isHelp() )
	{
		parser.showHelp( std::cout );
		return EXIT_SUCCESS;
	}
	if( parser.isVersion() )
	{
		showVersion();
		return EXIT_SUCCESS;
	}

	auto kindsOpt = parser.getReporterKinds();

	ProjectPtr projectPtr = createProject( parser );
	if( !projectPtr )
	{
		throw CantCreateProjectImpl();
	}

	Project & project = *projectPtr;
	if( parser.isVerbose() )
		dump( project );

	ModelPtr modelPtr = runAnalyzer( project );
	if( !modelPtr )
	{
		throw CantCreateModelImpl();
	}
	Model & model = *modelPtr;

	const int maxFiles = getReportLimit( parser );
	const int maxDetails = getReportDetailsLimit( parser );

	if( kindsOpt )
		runReporters( model, *kindsOpt, maxFiles, maxDetails );
	else
		runReporters( model, parser.getDefaultReporterKinds(), maxFiles, maxDetails );

	return EXIT_SUCCESS;

}

//------------------------------------------------------------------------------

ConcoleApplication::ProjectPtr ConcoleApplication::createProject(
	const ParserArgWrapper & _arguments
)
{
	getLog().printLine( resources::messages::StartConfigurateProject );

	ProjectBuilder projectBuilder(
		ensureProjectAccessor(),
		ensureJsonAccessor(),
		ensureFileSystem()
	);
	return projectBuilder.build( _arguments );
}

//------------------------------------------------------------------------------

ConcoleApplication::ModelPtr ConcoleApplication::runAnalyzer(
	const Project & _project
)
{
	getLog().printLine( resources::messages::StartAnalyzeSources );

	AnalyzerPtr analyzerPtr = createAnalyzer();
	if( !analyzerPtr )
	{
		throw CantCreateAnalyzerImpl();
		return nullptr;
	}
	return analyzerPtr->analyze( _project );
}

//------------------------------------------------------------------------------

int ConcoleApplication::getReportLimit( const ParserArgWrapper & _arg ) const
{
	int result = 0;
	if( auto limitOpt = _arg.getReportLimit(); limitOpt )
	{
		result = *limitOpt;
	}
	else
	{
		result = _arg.getDefaultReportLimit();
	}
	return result;
}

//------------------------------------------------------------------------------

int ConcoleApplication::getReportDetailsLimit( const ParserArgWrapper & _arg ) const
{
	int result = 0;
	if( auto limitOpt = _arg.getReportDetailsLimit(); limitOpt )
	{
		result = *limitOpt;
	}
	else
	{
		result = _arg.getDefaultReportDetailsLimit();
	}
	return result;
}

//------------------------------------------------------------------------------

void ConcoleApplication::runReporters(
	const Model & _model,
	const ReporterKinds & _kinds,
	int _limit,
	int _maxDetails
)
{
	using namespace reporter;

	getLog().printLine( resources::messages::StartPrintResult );

	Factory & factory = ensureReporterAccessor().getReporterFactory();

	for( reporter::ReporterKind kind : _kinds )
	{
		auto reportPtr = factory.createReporter( kind );
		if( !reportPtr )
		{
			getLog().printWarning( CantCreateReportImpl{ kind } );
			continue;
		}

		Report & report = *reportPtr;
		report.setMaxFilesCount( _limit );
		report.setMaxDetailsCount( _maxDetails );
		report.report( _model, std::cout );
	}
}

//------------------------------------------------------------------------------

ConcoleApplication::ProjectAccessor & ConcoleApplication::ensureProjectAccessor()
{
	return m_projectAccessor.ensure< project::ProjectAccessorImpl >();
}

//------------------------------------------------------------------------------

ConcoleApplication::ModelIncludesAccessor &
ConcoleApplication::ensureModelIncludesAccessor()
{
	return m_modelIncludesAccessor.ensure<
		model_includes::ModelIncludesAccessorImpl
	>();
}

//------------------------------------------------------------------------------

ConcoleApplication::AnalyzerPtr ConcoleApplication::createAnalyzer()
{
	fs::FileSystem & fs = ensureFileSystem();
	const parser::Parser & parser = ensureParser();
	auto & accessor  = ensureModelIncludesAccessor();
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

ConcoleApplication::ReporterAccessor & ConcoleApplication::ensureReporterAccessor()
{
	return m_reporterAccessor.ensure< reporter::ReporterAccessorImpl >();
}

//------------------------------------------------------------------------------

ConcoleApplication::JsonAccessor & ConcoleApplication::ensureJsonAccessor()
{
	return m_jsonAccessor.ensure< json::JsonAccesorImpl >();
}

//------------------------------------------------------------------------------

Log & ConcoleApplication::getLog()
{
	if( !m_log )
	{
		m_log.reset( new Log{ std::cout, std::cerr } );
	}
	return *m_log;
}

//------------------------------------------------------------------------------

void ConcoleApplication::dump( const Project & _project ) const
{
	std::cout << "project directory : " << _project.getProjectDir() << std::endl;

	const Project::IncludeDirIndex includeDirsCount = _project.getIncludeDirsCount();
	if( includeDirsCount )
	{
		std::cout << "Include dirs:\n";
		for( Project::IncludeDirIndex i = 0; i < includeDirsCount; ++i )
		{
			std::cout << "\t" << i + 1 << " : " << _project.getIncludeDir( i ) << std::endl;
		}
	}
	int ignoreDirNumber = 1;
	_project.forEachIgnoreDir(
		[&]( const std::filesystem::path & _path )
		{
			if( ignoreDirNumber == 1 )
			{
				std::cout << "Ignore dirs:\n";
			}

			std::cout << "\t" << ignoreDirNumber << " : " << _path << std::endl;
			++ignoreDirNumber;
			return true;
		}
	);

	int fileExtentionNumber = 1;
	_project.forEachFileExtention(
		[&]( std::string_view _ext )
		{
			if( fileExtentionNumber == 1 )
			{
				std::cout << "File extensions\n";
			}

			std::cout << "\t" << fileExtentionNumber << " : " << _ext << std::endl;

			++fileExtentionNumber;
			return true;
		}
	);

}

//------------------------------------------------------------------------------

void ConcoleApplication::showVersion()
{
	using namespace resources;

	std::cout << fmt::format(
		version::VersionFormat,
		version::Major,
		version::Minor,
		version::Path
	);
}

//------------------------------------------------------------------------------

}
