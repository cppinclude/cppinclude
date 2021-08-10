#pragma once

#include "application/tools/app_plugin_ptr.hpp"

#include <std_fs>
#include <stdfwd/set>
#include <stdfwd/string>
#include <stdfwd/string_view>
#include <stdfwd/vector>

#include <memory>

//------------------------------------------------------------------------------

namespace project
{
class ProjectAccessor;
class Project;
}

namespace json
{
class JsonAccessor;
}

namespace fs
{
class FileSystemAccessor;
class FileSystem;
class File;
}

namespace application
{
class ParserArgWrapper;
class ProjectBuilder;
class ConfigurationFile;
}

//------------------------------------------------------------------------------

namespace application::test
{
//------------------------------------------------------------------------------

class ProjectBuilderFixture
{
public:
	using Arguments = stdfwd::vector< std::string >;
	using Strings = stdfwd::vector< std::string >;
	using OrderedStrings = stdfwd::set< std::string >;
	using FilePtr = std::shared_ptr< fs::File >;
	using Path = stdfs::path;

	ProjectBuilderFixture();
	~ProjectBuilderFixture();

	void parserArguments( const Arguments & _arguments );
	void buildProject();

	const project::Project & getProject() const;

	std::string getProjectDir() const;

	std::string getIncludeDirs() const;
	std::string getIgnoreDirs() const;

	std::size_t getFileFiltersCount() const;

	bool getIgnoreSystemIncludes() const;
	std::string getFileExtensions() const;
	bool getAnalyzeWithoutExtension() const;

	Path getProjectPath() const;
	std::string toAbsolutePath( std::string_view _path );

	void createDefaultConfigurationFile( std::string_view _text );

	static Strings toStrings( const char * const _values[] );
	static std::string toString( const char * const _values[] );

private:
	template< class _Strings >
	static std::string toString( const _Strings & _strings );

	ConfigurationFile * loadConfigurationFile();

	ParserArgWrapper & getArgumentParser();
	ProjectBuilder & getBuilder();

	project::ProjectAccessor & getProjectAccessor();
	json::JsonAccessor & getJsonAccessor();
	fs::FileSystem & getFileSystem();

	fs::File & getDefaultConfigurationFile();

private:
	std::unique_ptr< project::Project > m_project;
	std::unique_ptr< ParserArgWrapper > m_parser;
	std::unique_ptr< ProjectBuilder > m_builder;
	std::unique_ptr< ConfigurationFile > m_configurationFile;

	PluginPtr< project::ProjectAccessor > m_projectAccessor;
	PluginPtr< json::JsonAccessor > m_json;
	PluginPtr< fs::FileSystemAccessor > m_fs;

	FilePtr m_defaultConfigurationFile;
};

//------------------------------------------------------------------------------

}
