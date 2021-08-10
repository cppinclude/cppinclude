#pragma once

#include "model_includes/api/mi_analyzer.hpp"

#include <std_fs>
#include <stdfwd/vector>

#include <memory>

//------------------------------------------------------------------------------

namespace fs
{
class FileSystem;
}

//------------------------------------------------------------------------------

namespace parser
{
class Parser;
class IncludeFile;
}

//------------------------------------------------------------------------------

namespace model_includes
{
class Model;
class File;
class Include;
class AnalyzerContext;
class Resolver;
enum class FileType;
enum class IncludeType;
enum class IncludeStatus;

//------------------------------------------------------------------------------

class AnalyzerImpl final : public Analyzer
{
public:
	AnalyzerImpl( const fs::FileSystem & _fs, const parser::Parser & _parser );

	ModelPtr analyze( const project::Project & _project ) const override;

	ModelPtr analyze(
		const project::Project & _project,
		const cmake_project::Project & _cmakeProject ) const override;

private:
	using Path = stdfs::path;
	using IncludeFiles = stdfwd::vector< parser::IncludeFile >;

	static ModelPtr initModel( const project::Project & _project );

	void
	analyzeFolder( AnalyzerContext & _context, const Path & _folderPath ) const;

	void analyzeFile( AnalyzerContext & _context, const Path & _path ) const;

	void analyzeIncludeFiles(
		AnalyzerContext & _context,
		const Path & _path,
		const IncludeFiles & _includesFile ) const;

	void analyzeIncludeFile(
		AnalyzerContext & _context,
		const Path & _path,
		const parser::IncludeFile & _includeFile,
		File & _file ) const;

	static void postProcesNewInclude(
		AnalyzerContext & _context, const model_includes::Include & _include );

	const Path & getProjectDir() const;

	using ResolvedPath = std::pair< Path, bool >;
	ResolvedPath resolvePath(
		const AnalyzerContext & _context,
		const Path & _currentFile,
		const parser::IncludeFile & _includeFile ) const;

	FileType getFileType( const Path & _path ) const;

	IncludeType
	getIncludeType( const parser::IncludeFile & _includesFile ) const;
	IncludeStatus
	getIncludeStatus( const File & _file, const ResolvedPath & _pair ) const;

	Resolver & ensureResolver() const;

private:
	const fs::FileSystem & m_fs;
	const parser::Parser & m_parser;
	mutable std::unique_ptr< Resolver > m_resolver;
};

//------------------------------------------------------------------------------

}
