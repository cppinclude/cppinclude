#include "project/impl/prj_project_impl.hpp"

#include "tools/path_string_tools.hpp"

#include "exception/ih/exc_internal_error.hpp"

#include <functional>
#include <regex>

//------------------------------------------------------------------------------

namespace project {

//------------------------------------------------------------------------------

ProjectImpl::ProjectImpl()
	:	m_ignoreSystemIncludes{ false }
{
}

//------------------------------------------------------------------------------

void ProjectImpl::setProjectDir( const Path & _path )
{
	m_projectDir = _path;
}

//------------------------------------------------------------------------------

const std::filesystem::path & ProjectImpl::getProjectDir() const
{
	return m_projectDir;
}

//------------------------------------------------------------------------------

ProjectImpl::IncludeDirIndex ProjectImpl::getIncludeDirsCount() const
{
	return m_includeDirs.size();
}

//------------------------------------------------------------------------------

const ProjectImpl::Path & ProjectImpl::getIncludeDir( IncludeDirIndex _index ) const
{
	return m_includeDirs.at( _index );
}

//------------------------------------------------------------------------------

void ProjectImpl::addIncludeDir( const Path & _path )
{
	m_includeDirs.push_back( _path );
}

//------------------------------------------------------------------------------

void ProjectImpl::addIncludeDirs( const DirPaths & _paths )
{
	for( const Path & path : _paths )
		addIncludeDir( path );
}

//------------------------------------------------------------------------------

bool ProjectImpl::hasIgnoreDirs() const
{
	return !m_ignoredDirs.empty();
}

//------------------------------------------------------------------------------

bool ProjectImpl::isIgnoredDir( const Path & _path ) const
{
	if( _path == getProjectDir() )
		return false;

	Path path = convertToDirPath( _path );

	const std::string dirName = path.parent_path().filename().string();
	INTERNAL_CHECK_WARRING( !dirName.empty() );
	if( dirName.empty() )
		return false;

	// ignore hidden folders
	if( dirName.at( 0 ) == '.' )
		return true;

	const bool isFound = m_ignoredDirs.count( path );
	return isFound;
}

//------------------------------------------------------------------------------

void ProjectImpl::addIgnoredDir( const Path & _path )
{
	m_ignoredDirs.insert( _path );
}

//------------------------------------------------------------------------------

void ProjectImpl::addIgnoredDirs( const DirPaths & _paths )
{
	for( const Path & path : _paths )
		addIgnoredDir( path );
}

//------------------------------------------------------------------------------

void ProjectImpl::forEachIgnoreDir( PathCallback _callback ) const
{
	for( const Path & path : m_ignoredDirs )
	{
		if( !_callback( path ) )
			break;
	}
}

//------------------------------------------------------------------------------

bool ProjectImpl::hasCppFileExtentions() const
{
	return !m_extentions.empty();
}

//------------------------------------------------------------------------------

bool ProjectImpl::isExistsCppExtention( std::string_view _ext ) const
{
	std::string ext{ _ext };
	bool result = m_extentions.count( ext );
	if( !result )
	{
		ext = "*" + ext;
		result = m_extentions.count( ext );
	}

	return result;
}

//------------------------------------------------------------------------------

void ProjectImpl::addCppFileExtention( std::string_view _ext )
{
	std::string ext{ _ext };
	m_extentions.insert( ext );
}

//------------------------------------------------------------------------------

void ProjectImpl::addCppFileExtentions( const Strings & _extentions )
{
	for( std::string_view ext : _extentions )
		addCppFileExtention( ext );
}

//------------------------------------------------------------------------------

void ProjectImpl::forEachFileExtention( FileExtentionCallback _callback ) const
{
	for( const std::string & ext : m_extentions )
	{
		if( !_callback( ext ) )
			break;
	}
}

//------------------------------------------------------------------------------

bool ProjectImpl::getIgnoreSystemIncludes() const
{
	return m_ignoreSystemIncludes;
}

//------------------------------------------------------------------------------

void ProjectImpl::setIgnoreSystemIncludes( bool _ignore )
{
	m_ignoreSystemIncludes = _ignore;
}

//------------------------------------------------------------------------------

ProjectImpl::FileFilterIndex ProjectImpl::getFileFilterCount() const
{
	return m_fileFilters.size();
}

//------------------------------------------------------------------------------

const std::regex & ProjectImpl::getFileFilter( FileFilterIndex _index ) const
{
	return m_fileFilters.at( _index );
}

//------------------------------------------------------------------------------

void ProjectImpl::addFileFilter( std::string_view _filter )
{
	std::string str{ _filter };
	m_fileFilters.push_back( std::regex( str ) );
}

//------------------------------------------------------------------------------

void ProjectImpl::addFileFilters( const Strings & _filters )
{
	for( const auto & filter : _filters )
		addFileFilter( filter );
}

//------------------------------------------------------------------------------

bool ProjectImpl::hasFileFilters() const
{
	return !m_fileFilters.empty();
}

//------------------------------------------------------------------------------

bool ProjectImpl::isIgnoredFile( const Path & _path ) const
{
	for( const std::regex & filter : m_fileFilters )
	{
		if( isIgnoredFile( _path, filter ) )
			return true;
	}
	return false;
}

//------------------------------------------------------------------------------

void ProjectImpl::changeAllPathsToAbsolute()
{
	for( Path & path : m_includeDirs )
		changeToProjectPath( path );

	std::vector< Path > newPath;
	newPath.reserve( m_ignoredDirs.size() );
	for( Path path : m_ignoredDirs )
	{
		changeToProjectPath( path );
		newPath.push_back( path );
	}
	m_ignoredDirs.clear();
	m_ignoredDirs.insert( newPath.begin(), newPath.end() );
}

//------------------------------------------------------------------------------

void ProjectImpl::changeAllPathsToAbsolute( const Path & _currentDir )
{
	changeToAbsolute( _currentDir, m_projectDir );

	changeAllPathsToAbsolute();
}

//------------------------------------------------------------------------------

ProjectImpl::Path ProjectImpl::convertToDirPath( const Path & _path ) const
{
	Path path = _path;
	path = path.lexically_normal();
	if( !path.filename().empty() )
	{
		// folder path should finish with path separator
		path /= "";
	}

	return path;
}

//------------------------------------------------------------------------------

void ProjectImpl::changeToAbsolute( const Path & _currentDir, Path & _path )
{
	if( !_path.is_absolute() )
		_path = _currentDir / _path;

	changeToProjectPath( _path );
}

//------------------------------------------------------------------------------

bool ProjectImpl::isIgnoredFile( const Path & _path, const std::regex & _filter)
{
	const std::string originStr = _path.string();
	const std::string unixStr = tools::toUnixPath( originStr );
	if( regex_search( unixStr, _filter ) )
	{
		return true;
	}
	else
	{
		const std::string windowsStr = tools::toWindowsPath( originStr );
		return regex_search( windowsStr, _filter );
	}
}

//------------------------------------------------------------------------------

void ProjectImpl::changeToProjectPath( Path & _path )
{
	if( !_path.is_absolute() )
		_path = getProjectDir() / _path;

	_path = convertToDirPath( _path );
}

//------------------------------------------------------------------------------

}
