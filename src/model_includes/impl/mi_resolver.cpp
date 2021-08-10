#include "model_includes/impl/mi_resolver.hpp"

#include "model_includes/api/enums/mi_file_type.hpp"
#include "model_includes/impl/mi_resolver_context.hpp"
#include "model_includes/impl/mi_std_library.hpp"

#include "fs/api/fs_exceptions.hpp"
#include "fs/api/fs_file_system.hpp"

#include "cmake_project/api/cprj_project.hpp"
#include "project/api/prj_project.hpp"

#include "exception/ih/exc_internal_error.hpp"

#include <functional>
#include <optional>
#include <std_fs>

//------------------------------------------------------------------------------

namespace model_includes
{
//------------------------------------------------------------------------------

Resolver::Resolver( const fs::FileSystem & _fs )
	: m_fs{ _fs }
{
}

//------------------------------------------------------------------------------

Resolver::PathOpt Resolver::resolvePath(
	const project::Project & _project,
	const cmake_project::Project * _cmakeProject,
	const Path & _startFile,
	stdfwd::string_view _fileName,
	PathOpt _currentCMakeSourceFile ) const
{
	ResolverContext context{
		_project, _cmakeProject, _startFile, _fileName,
		std::move( _currentCMakeSourceFile ) };

	PathOpt pathOpt = checkInCurrentDir( context );
	if( pathOpt )
	{
		return pathOpt;
	}

	pathOpt = findInIncludeFolders( context );

	return pathOpt;
}

//------------------------------------------------------------------------------

FileType Resolver::resolveFileType( const Path & _file )
{
	FileType result = FileType::ProjectFile;

	if( _file.has_parent_path() )
	{
		return result;
	}

	const std::string name = _file.string();
	const StdLibrary & library = getStdLibrary();
	const bool isStdLib = library.isExists( name );
	result = isStdLib ? FileType::StdLibraryFile : FileType::ProjectFile;

	return result;
}

//------------------------------------------------------------------------------

Resolver::PathOpt
Resolver::checkInCurrentDir( const ResolverContext & _context ) const
{
	Path startDir = _context.getStartFile().parent_path();
	if( startDir.is_relative() )
	{
		startDir = _context.getProject().getProjectDir() / startDir;
	}

	const Path file = startDir / _context.getFileName();
	if( isExistFile( file ) )
	{
		return PathOpt{ file };
	}

	return std::nullopt;
}

//------------------------------------------------------------------------------

Resolver::PathOpt
Resolver::findInIncludeFolders( const ResolverContext & _context ) const
{
	PathOpt pathOpt = findInIncludeFoldersInProject( _context );
	if( pathOpt )
	{
		return pathOpt;
	}

	if( _context.getCMakeProject() != nullptr )
	{
		pathOpt = findInIncludeFoldersInCMakeProject( _context );
	}

	return pathOpt;
}

//------------------------------------------------------------------------------

Resolver::PathOpt Resolver::findInIncludeFoldersInProject(
	const ResolverContext & _context ) const
{
	const std::string & fileName = _context.getFileName();
	const project::Project & project = _context.getProject();
	const project::Project::IncludeDirIndex count =
		project.getIncludeDirsCount();
	const auto & projectDir = project.getProjectDir();

	for( project::Project::IncludeDirIndex i = 0; i < count; ++i )
	{
		const Path & includeDir = project.getIncludeDir( i );

		PathOpt fileOpt = findFile( projectDir, includeDir, fileName );
		if( fileOpt )
		{
			return fileOpt;
		}
	}

	return std::nullopt;
}

//------------------------------------------------------------------------------

Resolver::PathOpt Resolver::findInIncludeFoldersInCMakeProject(
	const ResolverContext & _context ) const
{
	const cmake_project::Project * projectOpt = _context.getCMakeProject();
	if( projectOpt == nullptr )
	{
		INTERNAL_CHECK_WARRING( false );
		return std::nullopt;
	}

	const cmake_project::Project & project = *projectOpt;
	const auto projectDir = _context.getProject().getProjectDir();
	const std::string & fileName = _context.getFileName();

	const Path & startFile = _context.getStartFile();

	PathOpt sourceFile = _context.getCurrentCMakeSourceFile();
	if( !sourceFile )
	{
		sourceFile = startFile;
	}

	PathOpt result;

	project.forEachIncludes( *sourceFile, [&]( const Path & _includedir ) {
		result = findFile( projectDir, _includedir, fileName );
		return !result;
	} );

	return result;
}

//------------------------------------------------------------------------------

Resolver::PathOpt Resolver::findFile(
	const Path & _projectDir,
	const Path & _includeDir,
	const std::string & _fileName ) const
{
	Path includeDir{ _includeDir };
	if( includeDir.is_relative() )
	{
		includeDir = _projectDir / includeDir;
	}

	Path file = includeDir / _fileName;

	file = stdfs::lexically_normal( file );

	if( isExistFile( file ) )
	{
		return PathOpt{ file };
	}

	return std::nullopt;
}

//------------------------------------------------------------------------------

bool Resolver::isExistFile( const Path & _filePath ) const
{
	try
	{
		return m_fs.isExistFile( _filePath );
	}
	catch( const fs::CheckingExistFileFail & _exception )
	{
		std::cout << _exception.what() << std::endl;
		return false;
	}
}

//------------------------------------------------------------------------------

const StdLibrary & Resolver::getStdLibrary()
{
	return StdLibrary::getInstance();
}

//------------------------------------------------------------------------------

}
