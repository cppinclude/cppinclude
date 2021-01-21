#include "cmake_project/test/fixtures/cprj_test_loader_fixture.hpp"

#include "cmake_project/api/cprj_loader.hpp"
#include "cmake_project/api/cprj_project.hpp"
#include "cmake_project/ih/cprj_accessor_impl.hpp"

#include "compilation_db/api/cdb_database.hpp"
#include "compilation_db/api/cdb_loader.hpp"
#include "compilation_db/ih/cdb_accessor_impl.hpp"

#include "exception/ih/exc_internal_error.hpp"

#include "tools/path_string_tools.hpp"

#include <std_fs>

#include <string_view>
#include <set>
#include <functional>

//------------------------------------------------------------------------------

namespace cmake_project::test {

//------------------------------------------------------------------------------

LoaderFixture::LoaderFixture() = default;
LoaderFixture::~LoaderFixture() = default;

//------------------------------------------------------------------------------

void LoaderFixture::addCommand(
	std::string_view _directory,
	std::string_view _command,
	std::string_view _file
)
{
	compilation_db::Database & db = ensureCompilationDb();
	db.addCommand( _directory, _command, _file );
}

//------------------------------------------------------------------------------

void LoaderFixture::addDefaultComandForFile( std::string_view _file )
{
	addCommand( getDefaultFolder(), "/usr/bin/c++", _file );
}

//------------------------------------------------------------------------------

void LoaderFixture::addComandWithIncludes(
	std::string_view _file,
	const Strings & _includes
)
{
	std::string includesArg;
	for( const auto & include : _includes )
		includesArg += " -I" + include;

	addCommand( getDefaultFolder(), "/usr/bin/c++" + includesArg, _file );
}

//------------------------------------------------------------------------------

void LoaderFixture::load()
{
	m_projectPtr = ensureLoader().load( ensureCompilationDb() );
}

//------------------------------------------------------------------------------

LoaderFixture::Strings LoaderFixture::getResultsFiles() const
{
	INTERNAL_CHECK_ERROR( m_projectPtr );
	const Project & project = *m_projectPtr;
	Strings files;
	project.forEachFilePath( [&]( const Path & _path )
	{
		files.insert( tools::toPath( _path.string() ) );
		return true;
	});
	return files;
}

//------------------------------------------------------------------------------

LoaderFixture::Strings LoaderFixture::getResultsIncludes(
	std::string_view _file
) const
{
	INTERNAL_CHECK_ERROR( m_projectPtr );
	const Project & project = *m_projectPtr;
	Strings files;
	const Path projectDir{ getDefaultFolder() };
	const Path filePath = projectDir / _file;

	project.forEachIncludes( filePath, [&]( const Path & _path )
	{
		files.insert( tools::toPath( _path.string() ) );
		return true;
	});
	return files;
}

//------------------------------------------------------------------------------

LoaderFixture::Strings LoaderFixture::toExceptedFiles( const Strings & _files ) const
{
	Strings result;
	Path projectDir{ getDefaultFolder() };
	for( const std::string & file : _files )
	{
		Path filePath = projectDir / file;
		result.insert( tools::toPath( filePath.string() ) );
	}
	return result;
}

//------------------------------------------------------------------------------

LoaderFixture::Strings LoaderFixture::toExceptedIncludes(
	const Strings & _dirs
) const
{
	Strings result;
	for( const std::string & dir : _dirs )
	{
		result.insert( tools::toPath( dir ) );
	}
	return result;
}

//------------------------------------------------------------------------------

std::string LoaderFixture::getDefaultFolder() const
{
	return "/project";
}

//------------------------------------------------------------------------------

Loader & LoaderFixture::ensureLoader()
{
	if( !m_loaderPtr )
		m_loaderPtr = ensureCmakeProjectAccessor().createLoader();

	return *m_loaderPtr;
}

//------------------------------------------------------------------------------

Accessor & LoaderFixture::ensureCmakeProjectAccessor()
{
	if( !m_cmakeProjectAccessorPtr )
		m_cmakeProjectAccessorPtr.reset( new AccessorImpl );

	return *m_cmakeProjectAccessorPtr;
}

//------------------------------------------------------------------------------

compilation_db::Database & LoaderFixture::ensureCompilationDb()
{
	if( !m_dbPtr )
		m_dbPtr = ensureCompilationDbLoader().createEmptyDb();

	return *m_dbPtr;
}

//------------------------------------------------------------------------------

compilation_db::Loader & LoaderFixture::ensureCompilationDbLoader()
{
	if( !m_dbLoaderPtr )
		m_dbLoaderPtr = ensureCompilationDbAccessor().createLoader();

	return *m_dbLoaderPtr;
}

//------------------------------------------------------------------------------

compilation_db::Accessor & LoaderFixture::ensureCompilationDbAccessor()
{
	if( !m_dbAccessorPtr )
		m_dbAccessorPtr.reset( new compilation_db::AccessorImpl );

	return *m_dbAccessorPtr;
}

//------------------------------------------------------------------------------

}
