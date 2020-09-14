#pragma once

#include "project/api/prj_project.hpp"

#include "tools/std_hash_fs_path.hpp"

#include <string>
#include <filesystem>
#include <vector>
#include <unordered_set>

//------------------------------------------------------------------------------

namespace project {

//------------------------------------------------------------------------------

class ProjectImpl final : public Project
{
public:

	explicit ProjectImpl();

	void setProjectDir( const Path & _path ) override;
	const Path & getProjectDir() const override;

	IncludeDirIndex getIncludeDirsCount() const override;
	const Path & getIncludeDir( IncludeDirIndex _index ) const override;
	void addIncludeDir( const Path & _path ) override;
	void addIncludeDirs( const DirPaths & _paths ) override;

	bool hasIgnoreDirs() const override;
	bool isIgnoredDir( const Path & _path ) const override;
	void addIgnoredDir( const Path & _path ) override;
	void addIgnoredDirs( const DirPaths & _paths ) override;
	void forEachIgnoreDir( PathCallback _callback ) const override;

	bool hasCppFileExtensions() const override;
	bool isExistsCppExtension( std::string_view _ext ) const override;

	void addCppFileExtension( std::string_view _ext ) override;
	void addCppFileExtensions( const Strings & _extensions ) override;
	void forEachFileExtension( FileExtensionCallback _callback ) const override;

	bool getAnalyzeWithoutExtension() const override;
	void setAnalyzeWithoutExtension( bool _enable ) override;

	bool getIgnoreSystemIncludes() const override;
	void setIgnoreSystemIncludes( bool _ignore ) override;

	FileFilterIndex getFileFilterCount() const override;
	const std::regex & getFileFilter( FileFilterIndex _index ) const override;
	void addFileFilter( std::string_view _filter ) override;
	void addFileFilters( const Strings & _filters ) override;
	bool hasFileFilters() const override;
	bool isIgnoredFile( const Path & _path ) const override;

	void changeAllPathsToAbsolute( const Path & _currentDir ) override;
	void changeAllPathsToAbsolute() override;

private:

	Path convertToDirPath( const Path & _path ) const;

	void changeToAbsolute( const Path & currentDir, Path & _path );
	void changeToProjectPath( Path & _path );

	static bool isIgnoredFile( const Path & _path, const std::regex & _filter );

private:

	std::vector< Path > m_includeDirs;
	std::unordered_set< Path > m_ignoredDirs;
	std::unordered_set< std::string > m_extensions;
	std::vector< std::regex > m_fileFilters;

	Path m_projectDir;

	bool m_ignoreSystemIncludes;
	bool m_analyzeWithoutExtension;
};

//------------------------------------------------------------------------------

}
