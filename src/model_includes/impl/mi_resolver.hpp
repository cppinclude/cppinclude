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

	using PathOpt = std::optional< std::filesystem::path >;

	Resolver(
		const fs::FileSystem & _fs,
		const project::Project & _project
	);

	PathOpt resolvePath(
		const std::filesystem::path & _startFile,
		stdfwd::string_view _fileName
	) const;

	static FileType resolveFileType( const std::filesystem::path & _startFile );

private:

	PathOpt checkInCurrentDir(
		const std::filesystem::path & _startFile,
		stdfwd::string_view _fileName
	) const;

	PathOpt findInIncludeFolders( stdfwd::string_view _fileName	) const;

	bool isExistFile( const std::filesystem::path & _filePath ) const;

	static const StdLibrary & getStdLibrary();

	const std::filesystem::path & getProjectFolder() const;

private:

	const fs::FileSystem & m_fs;
	const project::Project & m_project;
};

//------------------------------------------------------------------------------

}
