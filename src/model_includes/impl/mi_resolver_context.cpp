#include "model_includes/impl/mi_resolver_context.hpp"

//------------------------------------------------------------------------------

namespace model_includes {

//------------------------------------------------------------------------------

ResolverContext::ResolverContext(
	const project::Project & _project,
	const cmake_project::Project * _cmakeProject,
	Path _startFile,
	stdfwd::string_view _fileName,
	PathOpt _currentCMakeSourceFile
)
	:	m_project{ _project }
	,	m_cmakeProject{ _cmakeProject }
	,	m_startFile{ std::move( _startFile ) }
	,	m_fileName{ _fileName }
	,	m_currentCMakeSourceFile{ std::move( _currentCMakeSourceFile ) }
{
}

//------------------------------------------------------------------------------

const project::Project & ResolverContext::getProject() const
{
	return m_project;
}

//------------------------------------------------------------------------------

const cmake_project::Project * ResolverContext::getCMakeProject() const
{
	return m_cmakeProject;
}

//------------------------------------------------------------------------------

const ResolverContext::Path & ResolverContext::getStartFile() const
{
	return m_startFile;
}

//------------------------------------------------------------------------------

const std::string & ResolverContext::getFileName() const
{
	return m_fileName;
}

//------------------------------------------------------------------------------

ResolverContext::PathOpt ResolverContext::getCurrentCMakeSourceFile() const
{
	return m_currentCMakeSourceFile;
}

//------------------------------------------------------------------------------

}
