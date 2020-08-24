#include "reporter/impl/rp_base_reporter_impl.hpp"

#include <string>
#include <filesystem>

//------------------------------------------------------------------------------

namespace reporter {

//------------------------------------------------------------------------------

BaseReporterImpl::BaseReporterImpl()
	:	m_maxFilesCount{ 0 }
	,	m_maxDetailsCount{ 0 }
{
}

//------------------------------------------------------------------------------

void BaseReporterImpl::setMaxFilesCount( size_t _count )
{
	m_maxFilesCount = _count;
}

//------------------------------------------------------------------------------

void BaseReporterImpl::setMaxDetailsCount( size_t _count )
{
	m_maxDetailsCount = _count;
}

//------------------------------------------------------------------------------

size_t BaseReporterImpl::getMaxFilesCount() const
{
	return m_maxFilesCount;
}

//------------------------------------------------------------------------------

size_t BaseReporterImpl::getMaxDetailsCount() const
{
	return m_maxDetailsCount;
}

//------------------------------------------------------------------------------

std::string BaseReporterImpl::getPathWithoutProject(
	const std::filesystem::path & _filePath,
	const std::filesystem::path & _dirPath
) const
{
	if( _dirPath.empty() )
		return _filePath.string();

	std::filesystem::path result = _filePath.lexically_relative( _dirPath );
	// if file out of project then return full path
	if( ( !result.empty() ) && (*result.begin()) != ".." )
		return result.string();
	else
	{
		return _filePath.string();
	}
}

//------------------------------------------------------------------------------

}
