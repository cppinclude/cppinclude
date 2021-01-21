#include "cmake_project/impl/cprj_loader_impl.hpp"

#include "cmake_project/impl/cprj_project_impl.hpp"
#include "cmake_project/impl/cprj_includes_parser.hpp"

#include "compilation_db/api/cdb_database.hpp"
#include "compilation_db/api/cdb_command_object.hpp"

#include <std_fs>

#include <memory>

//------------------------------------------------------------------------------

namespace cmake_project {

//------------------------------------------------------------------------------

LoaderImpl::LoaderImpl() = default;
LoaderImpl::~LoaderImpl() = default;

//------------------------------------------------------------------------------

LoaderImpl::ProjectPtr LoaderImpl::load( const compilation_db::Database & _db )
{
	ProjectPtr result{ createEmptyProject() };

	const compilation_db::Database::IndexType count = _db.getCount();
	for( compilation_db::Database::IndexType i = 0; i < count; ++i )
	{
		const compilation_db::CommandObject & command = _db.getObject( i );

		const Path file = loadFile( command, *result );
		loadCommand( command, file,  *result );
	}

	return result;
}

//------------------------------------------------------------------------------

LoaderImpl::ProjectPtr LoaderImpl::createEmptyProject()
{
	return ProjectPtr{ new ProjectImpl };
}

//------------------------------------------------------------------------------

LoaderImpl::Path LoaderImpl::loadFile(
	const compilation_db::CommandObject & _commnad,
	Project & _project
)
{
	const std::string & fileStr = _commnad.getFile();
	Path filePath{ fileStr };
	if( filePath.is_relative() )
	{
		const std::string & dirStr = _commnad.getDirectory();
		stdfs::path dirPath{ dirStr };
		filePath = dirPath / filePath;
	}
	filePath = stdfs::lexically_normal( filePath );

	_project.addFilePath( filePath );
	return filePath;
}

//------------------------------------------------------------------------------

IncludesParser & LoaderImpl::ensureIncludesParser()
{
	if( !m_includeParser )
		m_includeParser.reset( new IncludesParser );

	return  *m_includeParser;
}

//------------------------------------------------------------------------------

void LoaderImpl::loadCommand(
	const compilation_db::CommandObject & _commnad,
	const Path & _currentFile,
	Project & _project
)
{
	IncludesParser & parser = ensureIncludesParser();
	const std::string & cmd = _commnad.getCommand();
	auto includes = parser.parse( cmd );
	for( const Path & include : includes )
	{
		_project.addIncludeToFile( _currentFile, include );
	}
}

//------------------------------------------------------------------------------

}
