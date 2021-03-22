#include "model_includes/impl/mi_analyzer_context.hpp"

#include "model_includes/api/mi_model.hpp"

#include "project/api/prj_project.hpp"

#include "parser/api/pr_include_file.hpp"

#include <std_fs>

#include <functional>
#include <optional>

//------------------------------------------------------------------------------

namespace model_includes {

//------------------------------------------------------------------------------

AnalyzerContext::AnalyzerContext(
	const project::Project & _project,
	Model & _model
)
	:	m_project{ _project }
	,	m_cmakeProject{ nullptr }
	,	m_model{ _model }
	,	needAnalyzeResolvedIncludes{ false }
{
}

//------------------------------------------------------------------------------

AnalyzerContext::AnalyzerContext(
	const project::Project & _project,
	const cmake_project::Project & _cmakeProject,
	Model & _model
)
	:	m_project{ _project }
	,	m_cmakeProject{ &_cmakeProject }
	,	m_model{ _model }
	,	needAnalyzeResolvedIncludes{ false }
{
}

//------------------------------------------------------------------------------

const project::Project & AnalyzerContext::getProject() const
{
	return m_project;
}

//------------------------------------------------------------------------------

const cmake_project::Project * AnalyzerContext::getCMakeProject() const
{
	return m_cmakeProject;
}

//------------------------------------------------------------------------------

const AnalyzerContext::Path & AnalyzerContext::getProjectDir() const
{
	return getProject().getProjectDir();
}

//------------------------------------------------------------------------------

bool AnalyzerContext::isIgnoredDir( const Path & _path ) const
{
	return getProject().isIgnoredDir( _path );
}

//------------------------------------------------------------------------------

bool AnalyzerContext::isCppFile( const Path & _path ) const
{
	const project::Project & project = getProject();

	if( !project.hasCppFileExtensions() )
	{
		return true;
	}

	const Path extension = _path.extension();

	if( extension.empty() && project.getAnalyzeWithoutExtension() )
	{
		return true;
	}

	const bool result = project.isExistsCppExtension( extension.string() );
	return result;
}

//------------------------------------------------------------------------------

bool AnalyzerContext::isIgnoredFile( const Path & _path ) const
{
	if( getModel().findFile( _path ) != nullptr )
	{
		return false;
	}

	if( m_ignoredFiles.count( _path ) > 0 )
	{
		return true;
	}

	if( getProject().isIgnoredFile( _path ) )
	{
		m_ignoredFiles.insert( _path );
		return true;
	}

	return false;
}

//------------------------------------------------------------------------------

bool AnalyzerContext::isIgnoreIncludeFile(
	const parser::IncludeFile & _includesFile
) const
{
	return getProject().getIgnoreSystemIncludes() && _includesFile.isSystem();
}

//------------------------------------------------------------------------------

bool AnalyzerContext::isFileInIgnoreDir( const Path & _path ) const
{
	const project::Project & project = getProject();
	bool isFound = false;
	project.forEachIgnoreDir( [&]( const Path & _ignoreDir )
	{
		isFound = isFolderHasFile( _ignoreDir, _path );
		return !isFound;
	});

	return isFound;
}

//------------------------------------------------------------------------------

bool AnalyzerContext::isFolderHasFile( const Path & _dir, const Path & _path )
{
	const std::string dirStr = _dir.string();
	const std::string pathStr = _path.string();

	const std::size_t dirSize = dirStr.size();
	const std::size_t pathSize = pathStr.size();

	if( dirSize >= pathSize )
	{
		return true;
	}

	const std::string pathSubStr = pathStr.substr( 0, dirSize );
	const bool result = ( pathSubStr == dirStr );
	return result;
}

//------------------------------------------------------------------------------

Model & AnalyzerContext::takeModel()
{
	return m_model;
}

//------------------------------------------------------------------------------

const Model & AnalyzerContext::getModel() const
{
	return m_model;
}

//------------------------------------------------------------------------------

void AnalyzerContext::setNeedAnalyzeResolvedIncludes( bool _enable )
{
	needAnalyzeResolvedIncludes = _enable;
}

//------------------------------------------------------------------------------

bool AnalyzerContext::isNeedAnalyzeResolvedIncludes() const
{
	return needAnalyzeResolvedIncludes;
}

//------------------------------------------------------------------------------

void AnalyzerContext::addResolvedFile( const Path & _path )
{
	auto pair = m_resolvedFiles.insert( _path );
	if( pair.second )
	{
		m_resolvedFilesStack.push( _path );
	}
}

//------------------------------------------------------------------------------

AnalyzerContext::PathOpt AnalyzerContext::popResolvedFile()
{
	if( m_resolvedFilesStack.empty() )
	{
		return std::nullopt;
	}

	Path result = m_resolvedFilesStack.top();
	m_resolvedFilesStack.pop();
	return result;
}

//------------------------------------------------------------------------------

void AnalyzerContext::setCurrentCMakeSourceFile( const Path & _path )
{
	m_currentCMakeSourceFile = _path;
}

//------------------------------------------------------------------------------

AnalyzerContext::PathOpt AnalyzerContext::getCurrentCMakeSourceFile() const
{
	return m_currentCMakeSourceFile;
}

//------------------------------------------------------------------------------

}
