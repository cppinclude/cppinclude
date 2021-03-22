#include "application/tools/app_project_builder.hpp"

#include "application/tools/app_configuration_file.hpp"
#include "application/tools/app_parser_arg_wrapper.hpp"

#include "project/api/prj_project.hpp"
#include "project/ih/prj_project_accessor.hpp"

#include "fs/api/fs_file_system.hpp"

#include "exception/ih/exc_internal_error.hpp"

#include <memory>
#include <optional>

//------------------------------------------------------------------------------

namespace application {

//------------------------------------------------------------------------------

ProjectBuilder::ProjectBuilder(
	ProjectAccessor & _projectAccessor,
	FileSystem & _fs
)
	:	m_projectAccessor{ _projectAccessor }
	,	m_fs{ _fs }
{
}

//------------------------------------------------------------------------------

ProjectBuilder::ProjectPtr ProjectBuilder::build(
	const ParserArgWrapper & _arguments,
	const ConfigurationFile * _configurationFile
)
{
	ProjectPtr projectPtr = createProject();
	if( !projectPtr )
	{
		THROW_INTERNAL_ERROR;
	}
	Project & project = *projectPtr;

	reset();

	if( _configurationFile != nullptr )
	{
		initProject( *_configurationFile, project );
	}

	initProject( _arguments, project );
	initProjectWithDefaultValues( _arguments, project );
	projectPtr->changeAllPathsToAbsolute( m_fs.getCurrentPath() );

	return projectPtr;
}

//------------------------------------------------------------------------------

void ProjectBuilder::initProjectWithDefaultValues(
	const ParserArgWrapper & _arguments,
	Project & _project
)
{
	if( auto projectDir = _project.getProjectDir(); projectDir.empty() )
	{
		_project.setProjectDir( _arguments.getDefaultProjectDir() );
	}

	if( !_project.hasCppFileExtensions() )
	{
		_project.addCppFileExtensions( _arguments.getDefaultFileExtensions() );
	}

	if( !m_analyzeWithoutExtensionChanged )
	{
		_project.setAnalyzeWithoutExtension(
			_arguments.getDefaultAnalyzeWithoutExtension()
		);
	}

	if( _project.getIncludeDirsCount() == 0U )
	{
		_project.addIncludeDirs( _arguments.getDefaultIncludeDirs() );
	}

	if( !_project.hasFileFilters() )
	{
		_project.addFileFilters( _arguments.getDefaultIgnoreFiles() );
	}

	if( !m_ignoreSystemIncludesChanged )
	{
		_project.setIgnoreSystemIncludes( _arguments.getDefaultIgnoreSystemIncludes() );
	}

	if( !_project.hasIgnoreDirs() )
	{
		_project.addIgnoredDirs( _arguments.getDefaultIgnoreDirs() );
	}
}

//------------------------------------------------------------------------------

template< class _Source >
void ProjectBuilder::initProject( const _Source & _source, Project & _project )
{
	if( auto dirOpt = _source.getProjectDir(); dirOpt )
	{
		Path dirPath = *dirOpt;
		if( !dirPath.is_absolute() )
		{
			dirPath = m_fs.toAbsolute( dirPath );
		}
		_project.setProjectDir( dirPath );
	}

	if( auto valueOpt = _source.getIncludeDirs(); valueOpt )
	{
		_project.addIncludeDirs( *valueOpt );
	}

	if( auto valueOpt = _source.getIgnoreDirs(); valueOpt )
	{
		_project.addIgnoredDirs( *valueOpt );
	}

	if( auto valueOpt = _source.getIgnoreFiles(); valueOpt )
	{
		_project.addFileFilters( *valueOpt );
	}

	if( auto valueOpt = _source.getIgnoreSystemIncludes(); valueOpt )
	{
		m_ignoreSystemIncludesChanged = true;
		_project.setIgnoreSystemIncludes( *valueOpt );
	}

	if( auto valueOpt = _source.getFileExtensions(); valueOpt )
	{
		_project.addCppFileExtensions( *valueOpt );
	}

	if( auto valueOpt = _source.getAnalyzeWithoutExtension(); valueOpt )
	{
		m_analyzeWithoutExtensionChanged = true;
		_project.setAnalyzeWithoutExtension( *valueOpt );
	}
}

//------------------------------------------------------------------------------

ProjectBuilder::ProjectPtr ProjectBuilder::createProject()
{
	return m_projectAccessor.createProject();
}

//------------------------------------------------------------------------------

void ProjectBuilder::reset()
{
	m_ignoreSystemIncludesChanged = false;
	m_analyzeWithoutExtensionChanged = false;
}

//------------------------------------------------------------------------------

}
