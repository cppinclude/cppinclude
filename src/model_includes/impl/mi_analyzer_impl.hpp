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

	using IncludeFiles = stdfwd::vector< parser::IncludeFile >;
	using IgnoredFiles = stdfwd::unordered_set< std::filesystem::path >;

	void analyzeFolder(
		const Project & _project,
		const std::filesystem::path & _path,
		Model & _model,
		IgnoredFiles & _ignoreFiles
	) const;
	
	void analyzeFile(
		const Project & _project,
		const std::filesystem::path & _path,
		Model & _model,
		IgnoredFiles & _ignoreFiles
	) const;

	void analyzeIncludeFiles(
		const Project & _project,
		const std::filesystem::path & _path,
		const IncludeFiles & _includesFile,
		Model & _model,
		IgnoredFiles & _ignoreFiles
	) const;

	void analyzeIncludeFile(
		const Project & _project,
		const std::filesystem::path & _path,
		const parser::IncludeFile & _includesFile,
		File & _file,
		Model & _model,
		IgnoredFiles & _ignoreFiles
	) const;

	bool isCppFile(
		const Project & _project,
		const std::filesystem::path & _path
	) const;

	const std::filesystem::path & getProjectDir() const;

	using ResolvedPath = std::pair< std::filesystem::path, bool >;
	ResolvedPath resolvePath(
		const Project & _project,
		const std::filesystem::path & _currentFile,
		const parser::IncludeFile & _includeFile
	) const;

	FileType getFileType( const std::filesystem::path & _path ) const;

	IncludeType getIncludeType( const parser::IncludeFile & _includesFile ) const;
	IncludeStatus getIncludeStatus(
		const File & _file,
		const ResolvedPath & _pair
	) const;

	static bool isIgnoredFile(
		const std::filesystem::path & _path,
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
