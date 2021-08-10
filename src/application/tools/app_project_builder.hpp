#pragma once

#include <std_fs>
#include <stdfwd/memory>

//------------------------------------------------------------------------------

namespace project
{
class ProjectAccessor;
class Project;
}

namespace fs
{
class FileSystem;
}

//------------------------------------------------------------------------------

namespace application
{
class ConfigurationFile;
class ParserArgWrapper;

//------------------------------------------------------------------------------

class ProjectBuilder
{
public:
	using ProjectAccessor = project::ProjectAccessor;
	using Project = project::Project;
	using ProjectPtr = stdfwd::unique_ptr< Project >;

	using Path = stdfs::path;
	using FileSystem = fs::FileSystem;

	ProjectBuilder( ProjectAccessor & _projectAccessor, FileSystem & _fs );

	ProjectPtr build(
		const ParserArgWrapper & _arguments,
		const ConfigurationFile * _configurationFile );

private:
	void initProjectWithDefaultValues(
		const ParserArgWrapper & _arguments, Project & _project );

	void initFromConfigurationFile( const Path & _file, Project & _project );

	template< class _Source >
	void initProject( const _Source & _source, Project & _project );

	ProjectPtr createProject();

	void reset();

private:
	ProjectAccessor & m_projectAccessor;
	FileSystem & m_fs;
	bool m_ignoreSystemIncludesChanged{};
	bool m_analyzeWithoutExtensionChanged{};
};

//------------------------------------------------------------------------------

}
