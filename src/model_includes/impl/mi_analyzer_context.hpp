#pragma once

#include "tools/std_hash_fs_path.hpp"

#include <std_fs>

#include <optional>
#include <stack>
#include <unordered_set>

//------------------------------------------------------------------------------

namespace project
{
class Project;
}

namespace cmake_project
{
class Project;
}

namespace parser
{
class IncludeFile;
}

//------------------------------------------------------------------------------

namespace model_includes
{
class Model;

//------------------------------------------------------------------------------

class AnalyzerContext
{
public:
	using Path = stdfs::path;
	using PathOpt = std::optional< Path >;
	using IgnoredFiles = std::unordered_set< Path >;

	using ResolvedFiles = std::unordered_set< Path >;
	using ResolvedFilesStack = std::stack< Path >;

	AnalyzerContext( const project::Project & _project, Model & _model );

	AnalyzerContext(
		const project::Project & _project,
		const cmake_project::Project & _cmakeProject,
		Model & _model );

	const project::Project & getProject() const;
	const cmake_project::Project * getCMakeProject() const;
	const Path & getProjectDir() const;

	const Model & getModel() const;
	Model & takeModel();

	bool isCppFile( const Path & _path ) const;
	bool isIgnoredDir( const Path & _path ) const;
	bool isIgnoredFile( const Path & _path ) const;
	bool isIgnoreIncludeFile( const parser::IncludeFile & _includesFile ) const;
	bool isFileInIgnoreDir( const Path & _path ) const;
	static bool isFolderHasFile( const Path & _dir, const Path & _path );

	void setNeedAnalyzeResolvedIncludes( bool _enable );
	bool isNeedAnalyzeResolvedIncludes() const;

	void addResolvedFile( const Path & _path );
	PathOpt popResolvedFile();

	void setCurrentCMakeSourceFile( const Path & _path );
	PathOpt getCurrentCMakeSourceFile() const;

private:
	const project::Project & m_project;
	const cmake_project::Project * m_cmakeProject;
	Model & m_model;

	mutable IgnoredFiles m_ignoredFiles;

	bool needAnalyzeResolvedIncludes;

	ResolvedFiles m_resolvedFiles;
	ResolvedFilesStack m_resolvedFilesStack;

	std::optional< Path > m_currentCMakeSourceFile;
};

//------------------------------------------------------------------------------

}
