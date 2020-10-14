#pragma once

#include "model_includes/api/mi_analyzer.hpp"

//------------------------------------------------------------------------------

namespace fs {
	class FileSystem;
}

//------------------------------------------------------------------------------

namespace parser {
	class Parser;
	class IncludeFile;
}

//------------------------------------------------------------------------------

namespace model_includes {
	class Model;
	class File;
	enum class FileType;
	enum class IncludeType;
	enum class IncludeStatus;

//------------------------------------------------------------------------------

class AnalyzerImpl final : public Analyzer
{
public:

	AnalyzerImpl(
		const fs::FileSystem & _fs,
		const parser::Parser & _parser
	);

	ModelPtr analyze( const Project & _project ) const override;

private:

	using Path = stdfs::path;
	using IncludeFiles = stdfwd::vector< parser::IncludeFile >;
	using IgnoredFiles = stdfwd::unordered_set< Path >;

	void analyzeFolder(
		const Project & _project,
		const Path & _path,
		Model & _model,
		IgnoredFiles & _ignoreFiles
	) const;
	
	void analyzeFile(
		const Project & _project,
		const Path & _path,
		Model & _model,
		IgnoredFiles & _ignoreFiles
	) const;

	void analyzeIncludeFiles(
		const Project & _project,
		const Path & _path,
		const IncludeFiles & _includesFile,
		Model & _model,
		IgnoredFiles & _ignoreFiles
	) const;

	void analyzeIncludeFile(
		const Project & _project,
		const Path & _path,
		const parser::IncludeFile & _includesFile,
		File & _file,
		Model & _model,
		IgnoredFiles & _ignoreFiles
	) const;

	bool isCppFile( const Project & _project, const Path & _path ) const;

	const Path & getProjectDir() const;

	using ResolvedPath = std::pair< Path, bool >;
	ResolvedPath resolvePath(
		const Project & _project,
		const Path & _currentFile,
		const parser::IncludeFile & _includeFile
	) const;

	FileType getFileType( const Path & _path ) const;

	IncludeType getIncludeType( const parser::IncludeFile & _includesFile ) const;
	IncludeStatus getIncludeStatus(
		const File & _file,
		const ResolvedPath & _pair
	) const;

	static bool isIgnoredFile(
		const Path & _path,
		const Project & _project,
		const Model & _model,
		IgnoredFiles & _ignoreFiles
	);

private:

	const fs::FileSystem & m_fs;
	const parser::Parser & m_parser;
};

//------------------------------------------------------------------------------

}
