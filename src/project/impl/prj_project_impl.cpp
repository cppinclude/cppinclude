#include "project/impl/prj_project_impl.hpp"

#include "project/impl/exceptions/prj_exception_invalid_regex.hpp"

#include "tools/path_string_tools.hpp"

#include "exception/ih/exc_internal_error.hpp"

#include <functional>
#include <regex>

//------------------------------------------------------------------------------

namespace project
{
//------------------------------------------------------------------------------

ProjectImpl::ProjectImpl()
	:	m_ignoreSystemIncludes{ false }
	,	m_analyzeWithoutExtension{ false }
{
}

//------------------------------------------------------------------------------

void ProjectImpl::setProjectDir( const Path & _path )
{
	m_projectDir = _path;
}

//------------------------------------------------------------------------------

const ProjectImpl::Path & ProjectImpl::getProjectDir() const
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
	{
		addIncludeDir( path );
	}
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
	{
		return false;
	}

	Path path = convertToDirPath( _path );

	const std::string dirName = path.parent_path().filename().string();
	INTERNAL_CHECK_WARRING( !dirName.empty() );
	if( dirName.empty() )
	{
		return false;
	}

	// ignore hidden folders
	if( dirName.at( 0 ) == '.' )
	{
		return true;
	}

	const bool isFound = m_ignoredDirs.count( path ) > 0;
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
	{
		addIgnoredDir( path );
	}
}

//------------------------------------------------------------------------------

void ProjectImpl::forEachIgnoreDir( PathCallback _callback ) const
{
	for( const Path & path : m_ignoredDirs )
	{
		if( !_callback( path ) )
		{
			break;
		}
	}
}

//------------------------------------------------------------------------------

bool ProjectImpl::hasCppFileExtensions() const
{
	return !m_extensions.empty();
}

//------------------------------------------------------------------------------

bool ProjectImpl::isExistsCppExtension( std::string_view _ext ) const
{
	std::string ext{ _ext };
	bool result = m_extensions.count( ext ) > 0;
	if( !result )
	{
		ext = "*" + ext;
		result = m_extensions.count( ext ) > 0;
	}

	return result;
}

//------------------------------------------------------------------------------

void ProjectImpl::addCppFileExtension( std::string_view _ext )
{
	std::string ext{ _ext };
	m_extensions.insert( ext );
}

//------------------------------------------------------------------------------

void ProjectImpl::addCppFileExtensions( const Strings & _extensions )
{
	for( std::string_view ext : _extensions )
	{
		addCppFileExtension( ext );
	}
}

//------------------------------------------------------------------------------

void ProjectImpl::forEachFileExtension( FileExtensionCallback _callback ) const
{
	for( const std::string & ext : m_extensions )
	{
		if( !_callback( ext ) )
		{
			break;
		}
	}
}

//------------------------------------------------------------------------------

bool ProjectImpl::getAnalyzeWithoutExtension() const
{
	return m_analyzeWithoutExtension;
}

//------------------------------------------------------------------------------

void ProjectImpl::setAnalyzeWithoutExtension( bool _enable )
{
	m_analyzeWithoutExtension = _enable;
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

void ProjectImpl::addFileFilter( std::string_view _filter )
{
	std::string str{ _filter };
	try
	{
		m_fileFilters.emplace_back( tools::Regex{ str } );
	}
	catch( const std::regex_error & _exception )
	{
		throw InvalidRegexImpl{ str, _exception };
	}
}

//------------------------------------------------------------------------------

void ProjectImpl::addFileFilters( const Strings & _filters )
{
	for( const auto & filter : _filters )
	{
		addFileFilter( filter );
	}
}

//------------------------------------------------------------------------------

bool ProjectImpl::hasFileFilters() const
{
	return !m_fileFilters.empty();
}

//------------------------------------------------------------------------------

bool ProjectImpl::isIgnoredFile( const Path & _path ) const
{
	for( const tools::Regex & filter : m_fileFilters )
	{
		if( isIgnoredFile( _path, filter ) )
		{
			return true;
		}
	}
	return false;
}

//------------------------------------------------------------------------------

void ProjectImpl::changeAllPathsToAbsolute()
{
	for( Path & path : m_includeDirs )
	{
		changeToProjectPath( path );
	}

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
	path = stdfs::lexically_normal( path );
	const std::string filename = path.filename().string();
	if( !stdfs::is_dir_filename( path ) )
	{
		// folder path should finish with path separator
		path /= "";
		if( filename == path.filename().string() )
		{
			path /= Path::string_type{ Path::preferred_separator };
		}
	}

	return path;
}

//------------------------------------------------------------------------------

void ProjectImpl::changeToAbsolute( const Path & _currentDir, Path & _path )
{
	if( !_path.is_absolute() )
	{
		_path = _currentDir / _path;
	}

	changeToProjectPath( _path );
}

//------------------------------------------------------------------------------

bool ProjectImpl::isIgnoredFile( const Path & _path, const tools::Regex & _filter )
{
	const std::string originStr = _path.string();
	const std::string unixStr = tools::toUnixPath( originStr );
	if( checkFilter( unixStr, _filter ) )
	{
		return true;
	}

	const std::string windowsStr = tools::toWindowsPath( originStr );
	return checkFilter( windowsStr, _filter );
}

//------------------------------------------------------------------------------

bool ProjectImpl::checkFilter(
	const std::string & _str,
	const tools::Regex  & _filter
)
{
	const bool result = _filter.search( _str );
	return result;
}

//------------------------------------------------------------------------------

void ProjectImpl::changeToProjectPath( Path & _path )
{
	if( !_path.is_absolute() )
	{
		_path = getProjectDir() / _path;
	}

	_path = convertToDirPath( _path );
}

//------------------------------------------------------------------------------

}
