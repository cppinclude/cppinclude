#pragma once

#include "cmake_project/api/cprj_project.hpp"

#include "tools/std_hash_fs_path.hpp"

#include <unordered_map>
#include <unordered_set>
#include <vector>

//------------------------------------------------------------------------------

namespace cmake_project {

//------------------------------------------------------------------------------

class ProjectImpl final : public Project
{
public:
	CountType getFilePathsCount() const override;
	void forEachFilePath( PathCallback _callback ) const override;
	void forEachIncludes(
		const Path & _file,
		PathCallback _callback
	) const override;

	void addFilePath( const Path & _path ) override;
	void addIncludeToFile( const Path & _file, const Path & _include ) override;

private:
	const Path & addInclude( const Path & _include );

private:

	using Files				= std::unordered_set< Path >;
	using Includes			= std::unordered_set< Path >;
	using IncludesForFile	= std::vector< const Path * >;
	using IncludesByFiles	= std::unordered_map< Path, IncludesForFile >;

	Files m_files;
	Includes m_includes;
	IncludesByFiles m_includesByFiles;
};

//------------------------------------------------------------------------------

}
