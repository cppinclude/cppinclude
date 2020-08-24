#include "model_includes/impl/mi_resolver.hpp"

#include "model_includes/impl/mi_std_library.hpp"
#include "model_includes/api/enums/mi_file_type.hpp"

#include "fs/api/fs_file_system.hpp"

#include "project/api/prj_project.hpp"

#include <optional>
#include <filesystem>

//------------------------------------------------------------------------------

namespace model_includes {

//------------------------------------------------------------------------------

Resolver::Resolver( const fs::FileSystem & _fs, const project::Project & _project  )
	:	m_fs{ _fs }
	,	m_project{ _project }
{
}

//------------------------------------------------------------------------------

const std::filesystem::path & Resolver::getProjectFolder() const
{
	return m_project.getProjectDir();
}

//------------------------------------------------------------------------------

Resolver::PathOpt Resolver::resolvePath(
	const std::filesystem::path & _startFile,
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

FileType Resolver::resolveFileType( const std::filesystem::path & _file )
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
	const std::filesystem::path & _startFile,
	stdfwd::string_view _fileName
) const
{
	std::filesystem::path startDir = _startFile.parent_path();
	if( startDir.is_relative() )
		startDir = getProjectFolder() / startDir;

	const std::filesystem::path file = startDir / _fileName;
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
		std::filesystem::path includeDir = m_project.getIncludeDir( i );
		if( includeDir.is_relative() )
			includeDir = getProjectFolder() / includeDir;

		const std::filesystem::path file = includeDir / _fileName;

		if( isExistFile( file ) )
			return PathOpt{ file };
	}

	return std::nullopt;
}

//------------------------------------------------------------------------------

bool Resolver::isExistFile( const std::filesystem::path & _filePath ) const
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
