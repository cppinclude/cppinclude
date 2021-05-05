#pragma once

#include "application/tools/app_plugin_ptr.hpp"

#include <stdfwd/memory>
#include <stdfwd/vector>

//------------------------------------------------------------------------------

namespace fs {
	class FileSystem;
	class FileSystemAccessor;
}

namespace parser {
	class ParserAccessor;
	class Parser;
}

namespace model_includes {
	class Model;
	class Analyzer;
	class ModelIncludesAccessor;
}

namespace project {
	class ProjectAccessor;
	class Project;
}

namespace cmake_project {
	class Project;
	class Accessor;
}

namespace compilation_db {
	class Accessor;
}

namespace reporter {
	class ReporterAccessor;
	class Reporter;
	class Settings;
	class Factory;
	enum class ReporterKind;
}

namespace json {
	class JsonAccessor;
}

//------------------------------------------------------------------------------

namespace application {
	class ParserArgWrapper;
	class ConfigurationFile;
	class Log;

//------------------------------------------------------------------------------

class ConcoleApplication
{
public:
	ConcoleApplication();
	~ConcoleApplication();

	int run( int _argc, char * _argv[] );

private:

	using ProjectAccessor		= project::ProjectAccessor;
	using Project				= project::Project;
	using ProjectPtr			= std::unique_ptr< Project >;

	using CMakeProject			= cmake_project::Project;
	using CMakeProjectPtr		= std::unique_ptr< CMakeProject >;
	using CMakeAccessor			= cmake_project::Accessor;

	using ModelIncludesAccessor = model_includes::ModelIncludesAccessor;
	using Model					= model_includes::Model;
	using ModelPtr				= std::unique_ptr< Model>;
	using Analyzer				= model_includes::Analyzer;
	using AnalyzerPtr			= std::unique_ptr< model_includes::Analyzer >;

	using ParserAccessor		= parser::ParserAccessor;
	using Parser				= parser::Parser;
	using ParserPtr				= std::unique_ptr< parser::Parser >;

	using ReporterAccessor		= reporter::ReporterAccessor;
	using Report				= reporter::Reporter;
	using ReportPtr				= std::unique_ptr< Report >;
	using ReporterKinds			= stdfwd::vector< reporter::ReporterKind >;

	using JsonAccessor			= json::JsonAccessor;

	using CompilationDbAccessor	= compilation_db::Accessor;

	using ConfigurationFilePtr	= std::unique_ptr< ConfigurationFile >;

	ModelPtr runAnalyzer(
		const Project & _project,
		const CMakeProject * _cmakeProject
	);

	void runReporters(
		const Model & _model,
		const ReporterKinds & _kinds,
		const reporter::Settings & _reporterSettings
	);

	ProjectPtr createProject(
		const ParserArgWrapper & _arguments,
		const ConfigurationFile * _configurationFile
	);

	CMakeProjectPtr createCMakeProject(
		const ParserArgWrapper & _arguments,
		const ConfigurationFile * _configurationFile
	);

	ConfigurationFilePtr loadConfigurationFile(
		const ParserArgWrapper & _arguments
	);

	ProjectAccessor & ensureProjectAccessor();

	ModelIncludesAccessor & ensureModelIncludesAccessor();
	AnalyzerPtr createAnalyzer();

	fs::FileSystemAccessor & ensureFileSystemAccessor();
	fs::FileSystem & ensureFileSystem();

	ParserAccessor & ensureParserAccessor();
	const Parser & ensureParser();

	ReporterAccessor & ensureReporterAccessor();
	reporter::Factory & getReporterFactory();

	JsonAccessor & ensureJsonAccessor();

	CMakeAccessor & ensureCMakeAccessor();
	CompilationDbAccessor & ensureCompilationDbAccessor();

	Log & getLog();

	void dump( const Project & _project ) const;
	void dump( const CMakeProject & _project ) const;

	void showVersion();

private:
	PluginPtr< ProjectAccessor > m_projectAccessor;
	PluginPtr< ModelIncludesAccessor > m_modelIncludesAccessor;
	PluginPtr< fs::FileSystemAccessor > m_fileSystemAccessor;
	PluginPtr< ParserAccessor > m_parserAccessor;
	PluginPtr< ReporterAccessor > m_reporterAccessor;
	PluginPtr< JsonAccessor > m_jsonAccessor;
	PluginPtr< CMakeAccessor > m_cmakeAccessor;
	PluginPtr< CompilationDbAccessor > m_compilationDbAccessor;

	ParserPtr m_parser;
	std::unique_ptr< Log > m_log;
};

//------------------------------------------------------------------------------

}
