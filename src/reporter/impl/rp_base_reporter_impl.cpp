#include "reporter/impl/rp_base_reporter_impl.hpp"

#include "reporter/api/rp_settings.hpp"

#include "reporter/resources/rp_report_resources.hpp"

#include "exception/ih/exc_internal_error.hpp"

#include <fmt/format.h>
#include <std_fs>

#include <string>

//------------------------------------------------------------------------------

namespace reporter {

//------------------------------------------------------------------------------

BaseReporterImpl::BaseReporterImpl( SettingsPtr && _settingsPtr )
	:	m_settings{ std::move( _settingsPtr ) }
{
}

//------------------------------------------------------------------------------

BaseReporterImpl::~BaseReporterImpl() = default;

//------------------------------------------------------------------------------

const Settings & BaseReporterImpl::getSettings() const
{
	INTERNAL_CHECK_ERROR( m_settings );
	return *m_settings;
}

//------------------------------------------------------------------------------

void BaseReporterImpl::copySettings( const Settings & _settings )
{
	INTERNAL_CHECK_ERROR( m_settings );
	m_settings->copy( _settings );
}

//------------------------------------------------------------------------------

size_t BaseReporterImpl::getMaxFilesCount() const
{
	return getSettings().getMaxFilesCount();
}

//------------------------------------------------------------------------------

size_t BaseReporterImpl::getMaxDetailsCount() const
{
	return getSettings().getMaxDetailsCount();
}

//------------------------------------------------------------------------------

bool BaseReporterImpl::getShowStdFiles() const
{
	return getSettings().getShowStdFiles();
}

//------------------------------------------------------------------------------

std::string BaseReporterImpl::getPathWithoutProject(
	const Path & _filePath,
	const Path & _dirPath
)
{
	if( _dirPath.empty() )
	{
		return _filePath.string();
	}

	if( !isFromSameDirectory( _filePath, _dirPath ) )
	{
		return _filePath.string();
	}

	Path result = stdfs::lexically_relative( _filePath, _dirPath );
	if( ( !result.empty() ) )
	{
		return result.string();
	}

	return _filePath.string();
}

//------------------------------------------------------------------------------

bool BaseReporterImpl::isFromSameDirectory(
	const Path & _path1,
	const Path & _path2
)
{
	const Path root = _path1.root_path();
	if( root != _path2.root_path() )
	{
		return false;
	}

	Path commonPath = getCommonPath( _path1, _path2 );
	const bool result = commonPath != root;
	return result;
}

//------------------------------------------------------------------------------

BaseReporterImpl::Path BaseReporterImpl::getCommonPath(
	const Path & _path1,
	const Path & _path2
)
{
	Path resutl;

	auto itCurrentFirst	= _path1.begin();
	auto itEndFirst		= _path1.end();

	auto itCurrentSecond	= _path2.begin();
	auto itEndSecond		= _path2.end();

	while(
		itCurrentFirst != itEndFirst &&
		itCurrentSecond != itEndSecond &&
		*itCurrentFirst == *itCurrentSecond
	)
	{
		resutl /= *itCurrentFirst;

		++itCurrentFirst;
		++itCurrentSecond;
	}

	return resutl;
}

//------------------------------------------------------------------------------

bool BaseReporterImpl::isLimitFiles( CountType _currentNumber ) const
{
	return isLimit( _currentNumber, getMaxFilesCount() );
}

//------------------------------------------------------------------------------

bool BaseReporterImpl::isLimitFilesWithOriginSize(
	CountType _currentNumber,
	CountType _originSize
) const
{
	return isLimitFiles( _currentNumber ) && _currentNumber - 1 != _originSize;
}

//------------------------------------------------------------------------------

void BaseReporterImpl::printFileLimitLine(
	CountType _filesCount,
	std::ostream & _stream
) const
{
	const CountType limit = getMaxFilesCount();
	_stream << fmt::format( resources::LimitLineFmt, limit, _filesCount );
}

//------------------------------------------------------------------------------

bool BaseReporterImpl::isLimitDetails( CountType _currentNumber ) const
{
	return isLimit( _currentNumber, getMaxDetailsCount() );
}

//------------------------------------------------------------------------------

void BaseReporterImpl::printDetailsLimitLine(
	CountType _detailsCount,
	std::ostream & _stream
) const
{
	const CountType limit = getMaxDetailsCount();
	_stream << fmt::format( resources::LimitDetailLineFmt, limit, _detailsCount );
}

//------------------------------------------------------------------------------

bool BaseReporterImpl::isLimit( CountType _currentNumber, CountType _limit )
{
	return ( _limit > 0 ) && ( _currentNumber > _limit );
}

//------------------------------------------------------------------------------

}
