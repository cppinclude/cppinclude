#pragma once

#include <stdfwd.hpp>

#include <memory>

//------------------------------------------------------------------------------

namespace cmake_project {
	class Project;
	class Accessor;
}

namespace compilation_db {
	class Accessor;
}

namespace fs {
	class FileSystem;
	class FileSystemAccessor;
}

namespace application {
	class CMakeProjectBuilder;
	class ParserArgWrapper;
	class ConfigurationFile;
}

namespace json {
	class JsonAccessor;
}

//------------------------------------------------------------------------------

namespace application::test {

//------------------------------------------------------------------------------

class CMakeProjectBuilderFixture
{
public:
	using Strings = stdfwd::set< std::string >;
	using Path = stdfs::path;

	CMakeProjectBuilderFixture();
	~CMakeProjectBuilderFixture();

	void buildProject();
	bool isInitializedProject() const;

	void parserArgument( std::string_view _argument );
	void addCMakeFile( std::string_view _path, std::string_view _text );
	void setCompilationFileInConfigurationFile( std::string_view _path );

	Strings getResultsFiles();
	Strings getResultIncludes( std::string_view _file );
	Strings toExceptedFiles( const Strings & _strings );

private:
	Path toRelativePath( std::string_view _path );

	CMakeProjectBuilder & ensureBuilder();
	ParserArgWrapper & ensureArguments();
	ConfigurationFile & ensureConfigurationFile();

	fs::FileSystem & ensureFileSystem();
	fs::FileSystemAccessor & ensureFileSystemAccessor();

	cmake_project::Accessor & ensureCMakeAccessor();

	compilation_db::Accessor & ensureCompilationDbAccessor();
	json::JsonAccessor & ensureJsonAccessor();

private:
	std::unique_ptr< CMakeProjectBuilder > m_builder;
	std::unique_ptr< ParserArgWrapper > m_arguments;
	std::unique_ptr< ConfigurationFile > m_configurationFile;

	std::unique_ptr< cmake_project::Project > m_cmakeProject;
	std::unique_ptr< cmake_project::Accessor > m_cmakeAccessor;

	std::unique_ptr< compilation_db::Accessor > m_compilationDbAccessor;
	std::unique_ptr< json::JsonAccessor > m_jsonAccessor;

	std::unique_ptr< fs::FileSystemAccessor > m_fsAccessor;
};

//------------------------------------------------------------------------------

}
