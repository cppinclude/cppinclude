#pragma once

#include "cmake_project/api/cprj_loader.hpp"

#include <std_fs>

#include <memory>

//------------------------------------------------------------------------------

namespace compilation_db
{
class CommandObject;
}

//------------------------------------------------------------------------------

namespace cmake_project
{
class IncludesParser;

//------------------------------------------------------------------------------

class LoaderImpl final : public Loader
{
public:
	using Path = stdfs::path;

	LoaderImpl();
	~LoaderImpl();

	ProjectPtr load( const compilation_db::Database & _db ) override;

	ProjectPtr createEmptyProject() override;

private:
	Path loadFile(
		const compilation_db::CommandObject & _commnad, Project & _project );

	void loadCommand(
		const compilation_db::CommandObject & _commnad,
		const Path & _currentFile,
		Project & _project );

	IncludesParser & ensureIncludesParser();

private:
	std::unique_ptr< IncludesParser > m_includeParser;
};

//------------------------------------------------------------------------------

}
