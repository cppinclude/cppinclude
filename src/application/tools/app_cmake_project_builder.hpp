#pragma once

#include <std_fs>
#include <stdfwd/memory>

//------------------------------------------------------------------------------

namespace compilation_db
{
class Database;
class Loader;
class Accessor;
}

namespace cmake_project
{
class Project;
class Loader;
class Accessor;
}

namespace fs
{
class FileSystem;
}

namespace json
{
class JsonObject;
class JsonAccessor;
}

//------------------------------------------------------------------------------

namespace application
{
class ConfigurationFile;
class ParserArgWrapper;

//------------------------------------------------------------------------------

class CMakeProjectBuilder
{
public:
	using CMakeProjectPtr = stdfwd::unique_ptr< cmake_project::Project >;
	using CMakeProjectLoaderPtr = stdfwd::unique_ptr< cmake_project::Loader >;

	using CompilationDbPtr = stdfwd::unique_ptr< compilation_db::Database >;
	using CompilationLoaderPtr = stdfwd::unique_ptr< compilation_db::Loader >;

	using Path = stdfs::path;

	using JsonPtr = stdfwd::unique_ptr< json::JsonObject >;

	CMakeProjectBuilder(
		cmake_project::Accessor & _cmakeAccessor,
		compilation_db::Accessor & _compilationDbAccessor,
		json::JsonAccessor & _jsonAccessor,
		fs::FileSystem & _fs );

	CMakeProjectPtr build(
		const ParserArgWrapper & _arguments,
		const ConfigurationFile * _configurationFile );

private:
	static Path getPathToCmake(
		const ParserArgWrapper & _arguments,
		const ConfigurationFile * _configurationFile );

	bool isExistFile( const Path & _path ) const;

	CMakeProjectPtr loadProject( const Path & _path ) const;
	CMakeProjectLoaderPtr createCmakeLoader() const;

	CompilationDbPtr loadCompilationDb( const Path & _path ) const;
	CompilationLoaderPtr createCompilationDbLoader() const;

	JsonPtr loadJson( const Path & _path ) const;

private:
	cmake_project::Accessor & m_cmakeAccessor;
	compilation_db::Accessor & m_compilationDbAccessor;
	json::JsonAccessor & m_jsonAccessor;

	fs::FileSystem & m_fs;
};

//------------------------------------------------------------------------------

}
