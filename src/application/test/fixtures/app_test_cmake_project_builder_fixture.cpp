#include "application/test/fixtures/app_test_cmake_project_builder_fixture.hpp"

#include "application/tools/app_cmake_project_builder.hpp"
#include "application/tools/app_configuration_file.hpp"
#include "application/tools/app_parser_arg_wrapper.hpp"

#include "cmake_project/api/cprj_project.hpp"
#include "cmake_project/ih/cprj_accessor_impl.hpp"

#include "compilation_db/ih/cdb_accessor_impl.hpp"
#include "json/ih/json_accessor_impl.hpp"

#include "fs/api/fs_factory.hpp"
#include "fs/api/fs_file.hpp"
#include "fs/api/fs_file_system.hpp"
#include "fs/ih/fs_accessor_impl.hpp"

#include "exception/ih/exc_internal_error.hpp"

#include "tools/path_string_tools.hpp"

#include <std_fs>

#include <functional>
#include <memory>
#include <set>
#include <string>
#include <string_view>
#include <vector>

//------------------------------------------------------------------------------

namespace application::test
{
//------------------------------------------------------------------------------

CMakeProjectBuilderFixture::CMakeProjectBuilderFixture() = default;
CMakeProjectBuilderFixture::~CMakeProjectBuilderFixture() = default;

//------------------------------------------------------------------------------

void CMakeProjectBuilderFixture::buildProject()
{
	CMakeProjectBuilder & builder = ensureBuilder();

	m_cmakeProject =
		builder.build( ensureArguments(), m_configurationFile.get() );
}

//------------------------------------------------------------------------------

bool CMakeProjectBuilderFixture::isInitializedProject() const
{
	return m_cmakeProject != nullptr;
}

//------------------------------------------------------------------------------

void CMakeProjectBuilderFixture::addCMakeFile(
	std::string_view _path, std::string_view _text )
{
	fs::FileSystem & fs = ensureFileSystem();
	auto filePtr = fs.createFile( toRelativePath( _path ) );
	INTERNAL_CHECK_ERROR( filePtr != nullptr );
	fs::File & file = *filePtr;
	file << _text;
}

//------------------------------------------------------------------------------

void CMakeProjectBuilderFixture::setCompilationFileInConfigurationFile(
	std::string_view _path )
{
	ConfigurationFile & configurationFile = ensureConfigurationFile();
	configurationFile.setCompileCommands( _path );
}

//------------------------------------------------------------------------------

CMakeProjectBuilderFixture::Strings
CMakeProjectBuilderFixture::getResultsFiles()
{
	INTERNAL_CHECK_ERROR( m_cmakeProject != nullptr );
	const cmake_project::Project & project = *m_cmakeProject;
	Strings result;
	project.forEachFilePath( [&]( const Path & _path ) {
		auto pair = result.insert( tools::toPath( _path.string() ) );
		INTERNAL_CHECK_ERROR( pair.second );
		return true;
	} );

	return result;
}

//------------------------------------------------------------------------------

CMakeProjectBuilderFixture::Strings
CMakeProjectBuilderFixture::getResultIncludes( std::string_view _file )
{
	Strings result;
	INTERNAL_CHECK_ERROR( m_cmakeProject != nullptr );
	m_cmakeProject->forEachIncludes( _file, [&]( const Path & _include ) {
		result.insert( tools::toPath( _include.string() ) );
		return true;
	} );

	return result;
}

//------------------------------------------------------------------------------

CMakeProjectBuilderFixture::Strings
CMakeProjectBuilderFixture::toExceptedFiles( const Strings & _strings )
{
	Strings result;
	for( const std::string & str: _strings )
	{
		result.insert( tools::toPath( str ) );
	}
	return result;
}

//------------------------------------------------------------------------------

CMakeProjectBuilderFixture::Path
CMakeProjectBuilderFixture::toRelativePath( std::string_view _path )
{
	return ensureFileSystem().getCurrentPath() / _path;
}

//------------------------------------------------------------------------------

void CMakeProjectBuilderFixture::parserArgument( std::string_view _argument )
{
	ParserArgWrapper & arguments = ensureArguments();

	std::vector< std::string > argumentsStr;
	argumentsStr.emplace_back( "./application" );
	argumentsStr.emplace_back( std::string{ _argument } );

	arguments.parse( argumentsStr );
}

//------------------------------------------------------------------------------

CMakeProjectBuilder & CMakeProjectBuilderFixture::ensureBuilder()
{
	if( !m_builder )
	{
		m_builder = std::make_unique< CMakeProjectBuilder >(
			ensureCMakeAccessor(), ensureCompilationDbAccessor(),
			ensureJsonAccessor(), ensureFileSystem() );
	}

	return *m_builder;
}

//------------------------------------------------------------------------------

ParserArgWrapper & CMakeProjectBuilderFixture::ensureArguments()
{
	if( !m_arguments )
	{
		m_arguments = std::make_unique< ParserArgWrapper >();
		m_arguments->init();
	}

	return *m_arguments;
}

//------------------------------------------------------------------------------

ConfigurationFile & CMakeProjectBuilderFixture::ensureConfigurationFile()
{
	if( !m_configurationFile )
	{
		m_configurationFile = std::make_unique< ConfigurationFile >();
	}

	return *m_configurationFile;
}

//------------------------------------------------------------------------------

fs::FileSystem & CMakeProjectBuilderFixture::ensureFileSystem()
{
	return ensureFileSystemAccessor().getFactory().getMemoryFileSystem();
}

//------------------------------------------------------------------------------

fs::FileSystemAccessor & CMakeProjectBuilderFixture::ensureFileSystemAccessor()
{
	if( !m_fsAccessor )
	{
		m_fsAccessor = std::make_unique< fs::FileSystemAccessorImpl >();
	}

	return *m_fsAccessor;
}

//------------------------------------------------------------------------------

cmake_project::Accessor & CMakeProjectBuilderFixture::ensureCMakeAccessor()
{
	if( !m_cmakeAccessor )
	{
		m_cmakeAccessor = std::make_unique< cmake_project::AccessorImpl >();
	}

	return *m_cmakeAccessor;
}

//------------------------------------------------------------------------------

compilation_db::Accessor &
CMakeProjectBuilderFixture::ensureCompilationDbAccessor()
{
	if( !m_compilationDbAccessor )
	{
		m_compilationDbAccessor =
			std::make_unique< compilation_db::AccessorImpl >();
	}

	return *m_compilationDbAccessor;
}

//------------------------------------------------------------------------------

json::JsonAccessor & CMakeProjectBuilderFixture::ensureJsonAccessor()
{
	if( !m_jsonAccessor )
	{
		m_jsonAccessor = std::make_unique< json::JsonAccesorImpl >();
	}

	return *m_jsonAccessor;
}

//------------------------------------------------------------------------------

}
