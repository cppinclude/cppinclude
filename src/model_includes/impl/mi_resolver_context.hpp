#pragma once

#include <stdfwd/string_view>

#include <optional>
#include <std_fs>
#include <string>

//------------------------------------------------------------------------------

namespace project
{
class Project;
}

namespace cmake_project
{
class Project;
}

//------------------------------------------------------------------------------

namespace model_includes
{
//------------------------------------------------------------------------------

class ResolverContext
{
public:
	using Path = stdfs::path;
	using PathOpt = std::optional< Path >;

	ResolverContext(
		const project::Project & _project,
		const cmake_project::Project * _cmakeProject,
		Path _startFile,
		stdfwd::string_view _fileName,
		PathOpt _currentCMakeSourceFile );

	const project::Project & getProject() const;
	const cmake_project::Project * getCMakeProject() const;

	const Path & getStartFile() const;
	const std::string & getFileName() const;

	PathOpt getCurrentCMakeSourceFile() const;

private:
	const project::Project & m_project;
	const cmake_project::Project * m_cmakeProject;

	const Path m_startFile;
	const std::string m_fileName;

	const PathOpt m_currentCMakeSourceFile;
};

//------------------------------------------------------------------------------

}
