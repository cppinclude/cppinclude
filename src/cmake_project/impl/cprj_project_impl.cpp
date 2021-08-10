#include "cmake_project/impl/cprj_project_impl.hpp"

#include "exception/ih/exc_internal_error.hpp"

#include <std_fs>

#include <functional>

//------------------------------------------------------------------------------

namespace cmake_project
{
//------------------------------------------------------------------------------

ProjectImpl::CountType ProjectImpl::getFilePathsCount() const
{
	return m_files.size();
}

//------------------------------------------------------------------------------

void ProjectImpl::forEachFilePath( PathCallback _callback ) const
{
	for( const Path & path: m_files )
	{
		if( !_callback( path ) )
		{
			break;
		}
	}
}

//------------------------------------------------------------------------------

void ProjectImpl::forEachIncludes(
	const Path & _file, PathCallback _callback ) const
{
	if( m_includesByFiles.count( _file ) == 0U )
	{
		return;
	}

	const IncludesForFile & includes = m_includesByFiles.at( _file );
	for( const Path * include: includes )
	{
		if( include == nullptr )
		{
			INTERNAL_CHECK_WARRING( false );
			continue;
		}

		if( !_callback( *include ) )
		{
			break;
		}
	}
}

//------------------------------------------------------------------------------

void ProjectImpl::addFilePath( const Path & _path )
{
	Path path = stdfs::lexically_normal( _path );
	m_files.insert( path );
}

//------------------------------------------------------------------------------

void ProjectImpl::addIncludeToFile( const Path & _file, const Path & _include )
{
	Path file = stdfs::lexically_normal( _file );
	IncludesForFile & includes = m_includesByFiles[file];

	const Path & include = addInclude( _include );
	includes.push_back( &include );
}

//------------------------------------------------------------------------------

const ProjectImpl::Path & ProjectImpl::addInclude( const Path & _include )
{
	auto pair = m_includes.insert( _include );
	return *pair.first;
}

//------------------------------------------------------------------------------

}
