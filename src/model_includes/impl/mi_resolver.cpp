#include "model_includes/impl/mi_resolver.hpp"

#include "model_includes/impl/mi_std_library.hpp"
#include "model_includes/api/enums/mi_file_type.hpp"

#include "fs/api/fs_file_system.hpp"

#include "project/api/prj_project.hpp"

#include <optional>
#include <std_fs>

//------------------------------------------------------------------------------

namespace model_includes {

//------------------------------------------------------------------------------

Resolver::Resolver( const fs::FileSystem & _fs, const project::Project & _project  )
	:	m_fs{ _fs }
	,	m_project{ _project }
{
}

//------------------------------------------------------------------------------

const Resolver::Path & Resolver::getProjectFolder() const
{
	return m_project.getProjectDir();
}

//------------------------------------------------------------------------------

Resolver::PathOpt Resolver::resolvePath(
	const Path & _startFile,
	stdfwd::string_view _fileName
) const
{
	if( PathOpt pathOpt = checkInCurrentDir( _startFile, _fileName ); pathOpt )
		return  pathOpt;

	if( PathOpt pathOpt = findInIncludeFolders( _fileName ); pathOpt )
		return  pathOpt;

	return std::nullopt;
}

//------------------------------------------------------------------------------

FileType Resolver::resolveFileType( const Path & _file )
{
	FileType result = FileType::ProjectFile;

	if( _file.has_parent_path() )
		return result;

	const std::string name = _file.string();
	const StdLibrary & library = getStdLibrary();
	const bool isStdLib = library.isExists( name );
	result = isStdLib ? FileType::StdLibraryFile : FileType::ProjectFile;

	return result;

}

//------------------------------------------------------------------------------

Resolver::PathOpt Resolver::checkInCurrentDir(
	const Path & _startFile,
	stdfwd::string_view _fileName
) const
{
	Path startDir = _startFile.parent_path();
	if( startDir.is_relative() )
		startDir = getProjectFolder() / startDir;

	const Path file = startDir / _fileName;
	if( isExistFile( file ) )
		return PathOpt{ file };

	return std::nullopt;

}

//------------------------------------------------------------------------------

Resolver::PathOpt Resolver::findInIncludeFolders( std::string_view _fileName ) const
{
	const project::Project::IncludeDirIndex count = m_project.getIncludeDirsCount();
	for( project::Project::IncludeDirIndex i = 0; i < count; ++i )
	{
		Path includeDir = m_project.getIncludeDir( i );
		if( includeDir.is_relative() )
			includeDir = getProjectFolder() / includeDir;

		const Path file = includeDir / _fileName;

		if( isExistFile( file ) )
			return PathOpt{ file };
	}

	return std::nullopt;
}

//------------------------------------------------------------------------------

bool Resolver::isExistFile( const Path & _filePath ) const
{
	return m_fs.isExistFile( _filePath );
}

//------------------------------------------------------------------------------

const StdLibrary & Resolver::getStdLibrary()
{
	return StdLibrary::getInstance();
}

//------------------------------------------------------------------------------

}
