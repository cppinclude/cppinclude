#include "project/test/fixture/prj_test_ficture.hpp"

#include "project/api/prj_project.hpp"

#include "project/ih/prj_project_accesso_impl.hpp"

#include "exception/ih/exc_internal_error.hpp"

#include <string>
#include <string_view>
#include <std_fs>
#include <vector>
#include <functional>
#include <algorithm>

//------------------------------------------------------------------------------

namespace project::test {

//------------------------------------------------------------------------------

ProjectFixture::ProjectFixture() = default;
ProjectFixture::~ProjectFixture() = default;

//------------------------------------------------------------------------------

Project & ProjectFixture::getProject()
{
	if( !m_project )
	{
		auto newProject{ getProjectAccessor().createProject() };
		m_project.swap( newProject );
	}
	return *m_project;
}

//------------------------------------------------------------------------------

const Project & ProjectFixture::getProject() const
{
	INTERNAL_CHECK_ERROR( m_project );
	return *m_project;
}

//------------------------------------------------------------------------------

void ProjectFixture::setProjectDir( const Path & _path )
{
	getProject().setProjectDir( _path );
}

//------------------------------------------------------------------------------

const ProjectFixture::Path & ProjectFixture::getProjectDir() const
{
	return getProject().getProjectDir();
}

//------------------------------------------------------------------------------

void ProjectFixture::addIncludeDirs( const Strings & _dirs )
{
	Paths paths{ toPaths( _dirs ) };
	getProject().addIncludeDirs( paths );
}

//------------------------------------------------------------------------------

ProjectFixture::Paths ProjectFixture::getIncludeDirs() const
{
	Paths result;

	const Project & project = getProject();
	const Project::IncludeDirIndex count = project.getIncludeDirsCount();
	for( Project::IncludeDirIndex i = 0; i < count; ++i )
	{
		const Path & path = project.getIncludeDir( i );
		result.push_back( path );
	}

	return result;
}
//------------------------------------------------------------------------------

void ProjectFixture::addIgnoreDirs( const Strings & _dirs )
{
	Paths paths{ toPaths( _dirs ) };
	getProject().addIgnoredDirs( paths );
}

//------------------------------------------------------------------------------

bool ProjectFixture::isIgnoreDir( std::string_view _path )
{
	return getProject().isIgnoredDir( std::string{ _path } );
}

//------------------------------------------------------------------------------

ProjectFixture::Paths ProjectFixture::getIgnoreDirs() const
{
	Paths result;
	getProject().forEachIgnoreDir(
		[&]( const Path & _path )
		{
			result.push_back( _path );
			return true;
		}
	);
	return result;
}

//------------------------------------------------------------------------------

void ProjectFixture::addFileFilter( std::string_view _filter )
{
	getProject().addFileFilter( _filter );
}

//------------------------------------------------------------------------------

bool ProjectFixture::isIgnoredFile( std::string_view _path ) const
{
	return getProject().isIgnoredFile( std::string( _path ) );
}

//------------------------------------------------------------------------------

void ProjectFixture::addCppExtensions( const Strings & _ext )
{
	getProject().addCppFileExtensions( _ext );
}

//------------------------------------------------------------------------------

bool ProjectFixture::isExistsCppExtension( std::string_view _ext ) const
{
	return getProject().isExistsCppExtension( _ext );
}

//------------------------------------------------------------------------------

void ProjectFixture::changeAllPathsToAbsolute()
{
	getProject().changeAllPathsToAbsolute( getCurrentPath() );
}

//------------------------------------------------------------------------------

ProjectFixture::Path ProjectFixture::toAbsolutePath( const Path & _path )
{
	return getCurrentPath() / _path;
}

//------------------------------------------------------------------------------

ProjectFixture::Path ProjectFixture::getCurrentPath()
{
	return Path{ "/tmp/test_project/" };
}

//------------------------------------------------------------------------------

std::string ProjectFixture::toString( const Strings & _strings )
{
	const std::string seperator = ",";
	std::string result;

	for( const std::string str : _strings )
	{
		if( !result.empty() )
			result += seperator;

		result += str;
	}

	return result;
}

//------------------------------------------------------------------------------

ProjectFixture::Paths ProjectFixture::toPaths( const Strings & _strings )
{
	Paths result;
	for( const std::string & str : _strings )
	{
		result.push_back( str );
	}
	return result;
}

//------------------------------------------------------------------------------

ProjectAccessor & ProjectFixture::getProjectAccessor()
{
	if( !m_accessor )
	{
		m_accessor.reset( new ProjectAccessorImpl );
	}
	return *m_accessor;
}

//------------------------------------------------------------------------------

}
