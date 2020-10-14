#pragma once

#include <stdfwd.hpp>

//------------------------------------------------------------------------------

namespace project {
	class ProjectAccessor;
	class Project;
}

//------------------------------------------------------------------------------

namespace json {
	class JsonAccessor;
	class JsonObject;
}

//------------------------------------------------------------------------------

namespace fs {
	class FileSystem;
}

//------------------------------------------------------------------------------

namespace application {
	class ConfigurationFile;
	class ParserArgWrapper;

//------------------------------------------------------------------------------

class ProjectBuilder
{
public:

	using ProjectAccessor		= project::ProjectAccessor;
	using Project				= project::Project;
	using ProjectPtr			= stdfwd::unique_ptr< Project >;

	using JsonAccessor			= json::JsonAccessor;
	using JsonObject			= json::JsonObject;
	using JsonObjectPtr			= stdfwd::unique_ptr< JsonObject >;

	using Path					= stdfs::path;
	using FileSystem			= fs::FileSystem;

	ProjectBuilder(
		ProjectAccessor & _projectAccessor,
		JsonAccessor & _jsonAccessor,
		FileSystem & _fs
	);

	ProjectPtr build( const ParserArgWrapper & _arguments );

private:

	void initProjectWithDefaultValues(
		const ParserArgWrapper & _arguments,
		Project & _project
	);

	void initFromConfigurationFile( const Path & _file,  Project & _project );

	template< class _Source >
	void initProject( const _Source & _source, Project & _project );

	ProjectPtr createProject();

	JsonObjectPtr createJson( const Path & _file );

private:

	ProjectAccessor & m_projectAccessor;
	JsonAccessor & m_jsonAccessor;
	FileSystem & m_fs;
	bool m_ignoreSystemIncludesChanged;
	bool m_analyzeWithoutExtensionChanged;

};

//------------------------------------------------------------------------------

}
