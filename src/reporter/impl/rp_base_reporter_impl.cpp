#include "reporter/impl/rp_base_reporter_impl.hpp"

#include <string>
#include <std_fs>

//------------------------------------------------------------------------------

namespace reporter {

//------------------------------------------------------------------------------

BaseReporterImpl::BaseReporterImpl()
	:	m_maxFilesCount{ 0 }
	,	m_maxDetailsCount{ 0 }
	,	m_showStdFile{ false }
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

void BaseReporterImpl::setShowStdFile( bool _enable )
{
	m_showStdFile = _enable;
}

//------------------------------------------------------------------------------

bool BaseReporterImpl::getShowStdFiles() const
{
	return m_showStdFile;
}

//------------------------------------------------------------------------------

std::string BaseReporterImpl::getPathWithoutProject(
	const Path & _filePath,
	const Path & _dirPath
) const
{
	if( _dirPath.empty() )
		return _filePath.string();

	Path result = stdfs::lexically_relative( _filePath, _dirPath );
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
