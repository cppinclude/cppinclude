#pragma once

#include <stdfwd.hpp>

//------------------------------------------------------------------------------

namespace fs {
	class FileSystem;
}

//------------------------------------------------------------------------------

namespace project {
	class Project;
}

//------------------------------------------------------------------------------

namespace model_includes {
	class StdLibrary;
	enum class FileType;

//------------------------------------------------------------------------------

class Resolver
{
public:

	using Path = stdfs::path;
	using PathOpt = std::optional< Path >;

	Resolver(
		const fs::FileSystem & _fs,
		const project::Project & _project
	);

	PathOpt resolvePath(
		const Path & _startFile,
		stdfwd::string_view _fileName
	) const;

	static FileType resolveFileType( const Path & _startFile );

private:

	PathOpt checkInCurrentDir(
		const Path & _startFile,
		stdfwd::string_view _fileName
	) const;

	PathOpt findInIncludeFolders( stdfwd::string_view _fileName	) const;

	bool isExistFile( const Path & _filePath ) const;

	static const StdLibrary & getStdLibrary();

	const Path & getProjectFolder() const;

private:

	const fs::FileSystem & m_fs;
	const project::Project & m_project;
};

//------------------------------------------------------------------------------

}
