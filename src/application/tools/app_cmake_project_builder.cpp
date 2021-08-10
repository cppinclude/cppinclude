#include "application/tools/app_cmake_project_builder.hpp"

#include "application/tools/app_configuration_file.hpp"
#include "application/tools/app_parser_arg_wrapper.hpp"

#include "compilation_db/api/cdb_database.hpp"
#include "compilation_db/api/cdb_loader.hpp"
#include "compilation_db/ih/cdb_accessor.hpp"

#include "cmake_project/api/cprj_loader.hpp"
#include "cmake_project/api/cprj_project.hpp"
#include "cmake_project/ih/cprj_accessor.hpp"

#include "exception/ih/exc_internal_error.hpp"

#include "json/api/json_object.hpp"
#include "json/ih/json_accessor.hpp"

#include "fs/api/fs_file.hpp"
#include "fs/api/fs_file_system.hpp"

#include <std_fs>

#include <memory>

//------------------------------------------------------------------------------

namespace application
{
//------------------------------------------------------------------------------

CMakeProjectBuilder::CMakeProjectBuilder(
	cmake_project::Accessor & _cmakeAccessor,
	compilation_db::Accessor & _compilationDbAccessor,
	json::JsonAccessor & _jsonAccessor,
	fs::FileSystem & _fs )
	: m_cmakeAccessor{ _cmakeAccessor }
	, m_compilationDbAccessor{ _compilationDbAccessor }
	, m_jsonAccessor{ _jsonAccessor }
	, m_fs{ _fs }
{
}

//------------------------------------------------------------------------------

CMakeProjectBuilder::CMakeProjectPtr CMakeProjectBuilder::build(
	const ParserArgWrapper & _arguments,
	const ConfigurationFile * _configurationFile )
{
	const Path path = getPathToCmake( _arguments, _configurationFile );
	if( isExistFile( path ) )
	{
		auto projectPtr = loadProject( path );
		INTERNAL_CHECK_WARRING( projectPtr );
		return projectPtr;
	}

	return nullptr;
}

//------------------------------------------------------------------------------

CMakeProjectBuilder::Path CMakeProjectBuilder::getPathToCmake(
	const ParserArgWrapper & _arguments,
	const ConfigurationFile * _configurationFile )
{
	if( auto pathOpt = _arguments.getCompileCommandsFile(); pathOpt )
	{
		return *pathOpt;
	}

	if( _configurationFile != nullptr )
	{
		if( auto pathOpt = _configurationFile->getCompileCommands(); pathOpt )
		{
			return *pathOpt;
		}
	}

	return _arguments.getDefaultCompileCommandsFile();
}

//------------------------------------------------------------------------------

bool CMakeProjectBuilder::isExistFile( const Path & _path ) const
{
	return m_fs.isExistFile( _path );
}

//------------------------------------------------------------------------------

CMakeProjectBuilder::CMakeProjectPtr
CMakeProjectBuilder::loadProject( const Path & _path ) const
{
	auto compilationDbPtr = loadCompilationDb( _path );
	INTERNAL_CHECK_WARRING( compilationDbPtr );
	if( !compilationDbPtr )
	{
		return nullptr;
	}

	auto loaderPtr = createCmakeLoader();
	INTERNAL_CHECK_WARRING( loaderPtr );
	if( !loaderPtr )
	{
		return nullptr;
	}

	return loaderPtr->load( *compilationDbPtr );
}

//------------------------------------------------------------------------------

CMakeProjectBuilder::CMakeProjectLoaderPtr
CMakeProjectBuilder::createCmakeLoader() const
{
	return m_cmakeAccessor.createLoader();
}

//------------------------------------------------------------------------------

CMakeProjectBuilder::CompilationDbPtr
CMakeProjectBuilder::loadCompilationDb( const Path & _path ) const
{
	auto jsonPtr = loadJson( _path );
	INTERNAL_CHECK_WARRING( jsonPtr );
	if( !jsonPtr )
	{
		return nullptr;
	}

	auto loaderPtr = createCompilationDbLoader();
	INTERNAL_CHECK_WARRING( loaderPtr );
	if( !loaderPtr )
	{
		return nullptr;
	}

	return loaderPtr->load( *jsonPtr );
}

//------------------------------------------------------------------------------

CMakeProjectBuilder::CompilationLoaderPtr
CMakeProjectBuilder::createCompilationDbLoader() const
{
	return m_compilationDbAccessor.createLoader();
}

//------------------------------------------------------------------------------

CMakeProjectBuilder::JsonPtr
CMakeProjectBuilder::loadJson( const Path & _path ) const
{
	auto filePtr = m_fs.openFile( _path );
	INTERNAL_CHECK_WARRING( filePtr );
	if( !filePtr )
	{
		return nullptr;
	}

	fs::File & file = *filePtr;

	JsonPtr json = m_jsonAccessor.createJson( file.toInputStream() );
	return json;
}

//------------------------------------------------------------------------------

}
