#include "application/tools/app_project_builder.hpp"

#include "application/tools/app_parser_arg_wrapper.hpp"
#include "application/tools/app_configuration_file.hpp"

#include "project/api/prj_project.hpp"
#include "project/ih/prj_project_accessor.hpp"

#include "json/api/json_object.hpp"
#include "json/ih/json_accessor.hpp"

#include "fs/api/fs_file_system.hpp"
#include "fs/api/fs_file.hpp"

#include "exception/ih/exc_internal_error.hpp"

#include <memory>
#include <optional>

//------------------------------------------------------------------------------

namespace application {

//------------------------------------------------------------------------------

ProjectBuilder::ProjectBuilder(
	ProjectAccessor & _projectAccessor,
	JsonAccessor & _jsonAccessor,
	FileSystem & _fs
)
	:	m_projectAccessor{ _projectAccessor }
	,	m_jsonAccessor{ _jsonAccessor }
	,	m_fs{ _fs }
{

}

//------------------------------------------------------------------------------

ProjectBuilder::ProjectPtr ProjectBuilder::build(
	const ParserArgWrapper & _arguments
)
{
	ProjectPtr projectPtr = createProject();
	if( !projectPtr )
	{
		THROW_INTERNAL_ERROR;
	}
	Project & project = *projectPtr;

	m_ignoreSystemIncludesChanges = false;

	auto configFileOpt = _arguments.getConfigurationFile();
	if( !configFileOpt )
		configFileOpt = _arguments.getDefaultConfigurationFile();

	if( configFileOpt )
		initFromConfigurationFile( *configFileOpt, project );

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
		_project.setProjectDir( _arguments.getDefaultProjectDir() );

	if( !_project.hasCppFileExtentions() )
		_project.addCppFileExtentions( _arguments.getDefaultFileExtensions() );

	if( !_project.getIncludeDirsCount() )
		_project.addIncludeDirs( _arguments.getDefaultIncludeDirs() );

	if( !_project.hasFileFilters() )
		_project.addFileFilters( _arguments.getDefaultIgnoreFiles() );

	if( !m_ignoreSystemIncludesChanges )
		_project.setIgnoreSystemIncludes( _arguments.getDefaultIgnoreSystemIncludes() );

	if( !_project.hasIgnoreDirs() )
		_project.addIgnoredDirs( _arguments.getDefaultIgnoreDirs() );
}

//------------------------------------------------------------------------------

template< class _Source >
void ProjectBuilder::initProject(
	const _Source & _source,
	Project & _project
)
{
	if( auto dirOpt = _source.getProjectDir(); dirOpt )
	{
		Path dirPath = *dirOpt;
		if( !dirPath.is_absolute() )
			dirPath = m_fs.toAbsolute( dirPath );
		_project.setProjectDir( dirPath );
	}

	if( auto valueOpt = _source.getIncludeDirs(); valueOpt )
		_project.addIncludeDirs( *valueOpt );

	if( auto valueOpt = _source.getIgnoreDirs(); valueOpt )
		_project.addIgnoredDirs( *valueOpt );

	if( auto valueOpt = _source.getIgnoreFiles(); valueOpt )
		_project.addFileFilters( *valueOpt );

	if( auto valueOpt = _source.getIgnoreSystemIncludes(); valueOpt )
	{
		m_ignoreSystemIncludesChanges = true;
		_project.setIgnoreSystemIncludes( *valueOpt );
	}

	if( auto valueOpt = _source.getFileExtensions(); valueOpt )
		_project.addCppFileExtentions( *valueOpt );
}

//------------------------------------------------------------------------------

void ProjectBuilder::initFromConfigurationFile(
	const std::filesystem::path & _file,
	Project & _project
)
{
	if( !m_fs.isExistFile( _file ) )
		return;

	JsonObjectPtr jsonObjectPtr = createJson( _file );
	INTERNAL_CHECK_WARRING( jsonObjectPtr )
	if( !jsonObjectPtr )
		return;

	ConfigurationFile configurationFile;
	configurationFile.loadFromJson( *jsonObjectPtr );
	initProject( configurationFile, _project );
}

//------------------------------------------------------------------------------

ProjectBuilder::ProjectPtr ProjectBuilder::createProject()
{
	return m_projectAccessor.createProject();
}

//------------------------------------------------------------------------------

ProjectBuilder::JsonObjectPtr ProjectBuilder::createJson(
	const std::filesystem::path & _file
)
{
	auto filePtr = m_fs.openFile( _file );
	INTERNAL_CHECK_WARRING( filePtr )
	if( !filePtr )
		return nullptr;

	return m_jsonAccessor.createJson( filePtr->toInputStream() );
}

//------------------------------------------------------------------------------

}
