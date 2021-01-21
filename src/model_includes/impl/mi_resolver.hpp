#pragma once

#include <stdfwd.hpp>

//------------------------------------------------------------------------------

namespace fs {
	class FileSystem;
}

namespace project {
	class Project;
}

namespace cmake_project {
	class Project;
}

//------------------------------------------------------------------------------

namespace model_includes {
	class StdLibrary;
	class ResolverContext;
	enum class FileType;

//------------------------------------------------------------------------------

class Resolver
{
public:

	using Path = stdfs::path;
	using PathOpt = std::optional< Path >;

	explicit Resolver( const fs::FileSystem & _fs );

	PathOpt resolvePath(
		const project::Project & _project,
		const cmake_project::Project * _cmakeProject,
		const Path & _startFile,
		stdfwd::string_view _fileName,
		PathOpt currentCMakeSourceFile
	) const;

	static FileType resolveFileType( const Path & _startFile );

private:

	PathOpt checkInCurrentDir( const ResolverContext & _context	) const;

	PathOpt findInIncludeFolders( const ResolverContext & _context ) const;
	PathOpt findInIncludeFoldersInProject(
		const ResolverContext & _context
	) const;
	PathOpt findInIncludeFoldersInCMakeProject(
		const ResolverContext & _context
	) const;

	PathOpt findFile(
		const Path & _projectDir,
		const Path & _includeDir,
		const std::string & _fileName
	) const;

	bool isExistFile( const Path & _filePath ) const;

	static const StdLibrary & getStdLibrary();

private:

	const fs::FileSystem & m_fs;
};

//------------------------------------------------------------------------------

}
