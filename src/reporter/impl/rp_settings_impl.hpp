#pragma once

#include "reporter/api/rp_settings.hpp"

//------------------------------------------------------------------------------

namespace reporter {

//------------------------------------------------------------------------------

class SettingsImpl final : public Settings
{
public:
	SettingsImpl();

	CountType getMaxFilesCount() const override;
	void setMaxFilesCount( CountType _count ) override;

	CountType getMaxDetailsCount() const override;
	void setMaxDetailsCount( CountType _count ) override;

	bool getShowStdFiles() const override;
	void setShowStdFiles( bool _enable ) override;

	bool getShowOnlyStdHeaders() const override;
	void setShowOnlyStdHeaders( bool _enable ) override;

	void copy( const Settings & _other ) override;

private:
	CountType m_maxFilesCount;
	CountType m_maxDetailsCount;
	bool m_showStdFiles;
	bool m_showOnlyStdHeaders;
};

//------------------------------------------------------------------------------

}
