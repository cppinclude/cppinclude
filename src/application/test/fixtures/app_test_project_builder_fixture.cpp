#include "application/test/fixtures/app_test_project_builder_fixture.hpp"

#include "application/tools/app_parser_arg_wrapper.hpp"
#include "application/tools/app_project_builder.hpp"
#include "application/resources/app_resources_arguments.hpp"

#include "project/api/prj_project.hpp"
#include "project/ih/prj_project_accesso_impl.hpp"

#include "json/ih/json_accessor_impl.hpp"

#include "fs/ih/fs_accessor_impl.hpp"
#include "fs/api/fs_file_system.hpp"
#include "fs/api/fs_factory.hpp"
#include "fs/api/fs_file.hpp"

#include "exception/ih/exc_internal_error.hpp"

#include <vector>
#include <string>
#include <filesystem>
#include <functional>
#include <set>

//------------------------------------------------------------------------------

namespace application::test {

//------------------------------------------------------------------------------

ProjectBuilderFixture::ProjectBuilderFixture() = default;
ProjectBuilderFixture::~ProjectBuilderFixture() = default;

//------------------------------------------------------------------------------

void ProjectBuilderFixture::parserArguments( const Arguments & _arguments )
{
	Arguments arguments{ _arguments };
	arguments.insert( arguments.begin(),  "./application" );

	std::vector< char * > arg;
	arg.reserve( arguments.size() );
	for( std::string & str : arguments )
		arg.push_back( str.data() );

	getArgumentParser().parse( static_cast< int >( arg.size() ), arg.data() );
}

//------------------------------------------------------------------------------

void ProjectBuilderFixture::buildProject()
{
	m_project = getBuilder().build( getArgumentParser() );
}

//------------------------------------------------------------------------------

const project::Project & ProjectBuilderFixture::getProject() const
{
	INTERNAL_CHECK_ERROR( m_project );
	return *m_project;
}

//------------------------------------------------------------------------------

std::string ProjectBuilderFixture::getProjectDir() const
{
	return getProject().getProjectDir().string();
}

//------------------------------------------------------------------------------

std::string ProjectBuilderFixture::getIncludeDirs() const
{
	Strings strings;
	const project::Project & project = getProject();
	const project::Project::IncludeDirIndex count = project.getIncludeDirsCount();
	for( project::Project::IncludeDirIndex i = 0; i < count; ++i )
	{
		const std::filesystem::path & path = project.getIncludeDir( i );
		strings.push_back( path.string() );
	}

	return toString( strings );
}

//------------------------------------------------------------------------------

std::string ProjectBuilderFixture::getIgnoreDirs() const
{
	OrderedStrings strings;
	const project::Project & project = getProject();
	project.forEachIgnoreDir(
		[&]( const std::filesystem::path & _path )
		{
			strings.insert( _path.string() );
			return true;
		}
	);
	return toString( strings );
}

//------------------------------------------------------------------------------

int ProjectBuilderFixture::getFileFiltersCount() const
{
	return getProject().getFileFilterCount();
}

//------------------------------------------------------------------------------

const std::regex & ProjectBuilderFixture::getFileFilter( int _index ) const
{
	return getProject().getFileFilter( _index );
}

//------------------------------------------------------------------------------

bool ProjectBuilderFixture::getIgnoreSystemIncludes() const
{
	return getProject().getIgnoreSystemIncludes();
}

//------------------------------------------------------------------------------

std::string ProjectBuilderFixture::getFileExtensions() const
{
	OrderedStrings strings;
	const project::Project & project = getProject();
	project.forEachFileExtension(
		[&]( const std::filesystem::path & _path )
		{
			strings.insert( _path.string() );
			return true;
		}
	);
	return toString( strings );
}

//------------------------------------------------------------------------------

bool ProjectBuilderFixture::getAnalyzeWithoutExtension() const
{
	return getProject().getAnalyzeWithoutExtension();
}

//------------------------------------------------------------------------------

std::filesystem::path ProjectBuilderFixture::getProjectPath() const
{
	return "/projects/test_project/";
}

//------------------------------------------------------------------------------

std::string ProjectBuilderFixture::toAbsolutePath( std::string_view _path )
{
	const std::filesystem::path currentPath =  getFileSystem().getCurrentPath();
	std::filesystem::path path = currentPath / _path;
	path = path.lexically_normal();
	return path.string();
}

//------------------------------------------------------------------------------

void ProjectBuilderFixture::createDefaultConfigurationFile(
	std::string_view _text
)
{
	getDefaultConfigurationFile() << _text;
}

//------------------------------------------------------------------------------

template< class _Strings >
std::string ProjectBuilderFixture::toString( const _Strings & _strings )
{
	std::string result;
	bool first = true;
	for( const std::string & str : _strings )
	{
		if( first )
			first = false;
		else
			result += ',';

		result += str;

	}
	return result;
}

//------------------------------------------------------------------------------

ProjectBuilderFixture::Strings ProjectBuilderFixture::toStrings(
	const char * const _values[]
)
{
	Strings result;
	const char * const * it = _values;
	while( *it )
	{
		const std::string str{ *it };
		result.push_back( str );
	}
	return result;
}

//------------------------------------------------------------------------------

std::string ProjectBuilderFixture::toString( const char * const _values[] )
{
	return toString( toStrings( _values ) );
}

//------------------------------------------------------------------------------

ParserArgWrapper & ProjectBuilderFixture::getArgumentParser()
{
	if( !m_parser )
	{
		m_parser.reset( new ParserArgWrapper );
		m_parser->init();
	}
	return *m_parser;
}

//------------------------------------------------------------------------------

ProjectBuilder & ProjectBuilderFixture::getBuilder()
{
	if( !m_builder )
	{
		m_builder.reset(
			new ProjectBuilder(
				getProjectAccessor(),
				getJsonAccessor(),
				getFileSystem()
			)
		);
	}
	return *m_builder;
}

//------------------------------------------------------------------------------

project::ProjectAccessor & ProjectBuilderFixture::getProjectAccessor()
{
	return m_projectAccessor.ensure< project::ProjectAccessorImpl >();
}

//------------------------------------------------------------------------------

json::JsonAccessor & ProjectBuilderFixture::getJsonAccessor()
{
	return m_json.ensure< json::JsonAccesorImpl >();
}

//------------------------------------------------------------------------------

fs::FileSystem & ProjectBuilderFixture::getFileSystem()
{
	return m_fs.ensure< fs::FileSystemAccessorImpl >()
		.getFactory().getMemoryFileSystem();
}

//------------------------------------------------------------------------------

fs::File & ProjectBuilderFixture::getDefaultConfigurationFile()
{
	if( !m_defaultConfigurationFile )
	{
		const std::filesystem::path filePath =
			getFileSystem().getCurrentPath() /
			resources::arguments::configurationFile::DefaultValue
		;
		m_defaultConfigurationFile = getFileSystem().createFile( filePath );
	}
	return *m_defaultConfigurationFile;
}

//------------------------------------------------------------------------------

}
