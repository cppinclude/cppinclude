#include "reporter/impl/rp_settings_impl.hpp"

//------------------------------------------------------------------------------

namespace reporter {

//------------------------------------------------------------------------------

SettingsImpl::SettingsImpl()
	:	m_maxFilesCount{ 0 }
	,	m_maxDetailsCount{ 0 }
	,	m_showStdFiles{ false }
	,	m_showOnlyStdHeaders{ false }
{
}

//------------------------------------------------------------------------------

SettingsImpl::CountType SettingsImpl::getMaxFilesCount() const
{
	return m_maxFilesCount;
}

//------------------------------------------------------------------------------

void SettingsImpl::setMaxFilesCount( CountType _count )
{
	m_maxFilesCount = _count;
}

//------------------------------------------------------------------------------

SettingsImpl::CountType SettingsImpl::getMaxDetailsCount() const
{
	return m_maxDetailsCount;
}

//------------------------------------------------------------------------------

void SettingsImpl::setMaxDetailsCount( CountType _count )
{
	m_maxDetailsCount = _count;
}

//------------------------------------------------------------------------------

bool SettingsImpl::getShowStdFiles() const
{
	return m_showStdFiles;
}

//------------------------------------------------------------------------------

void SettingsImpl::setShowStdFiles( bool _enable )
{
	m_showStdFiles = _enable;
}

//------------------------------------------------------------------------------

bool SettingsImpl::getShowOnlyStdHeaders() const
{
	return m_showOnlyStdHeaders;
}

//------------------------------------------------------------------------------

void SettingsImpl::setShowOnlyStdHeaders( bool _enable )
{
	m_showOnlyStdHeaders = _enable;
}

//------------------------------------------------------------------------------

void SettingsImpl::copy( const Settings & _other )
{
	setMaxFilesCount( _other.getMaxFilesCount() );
	setMaxDetailsCount( _other.getMaxDetailsCount() );
	setShowStdFiles( _other.getShowStdFiles() );
	setShowOnlyStdHeaders( _other.getShowOnlyStdHeaders() );
}

//------------------------------------------------------------------------------

}
