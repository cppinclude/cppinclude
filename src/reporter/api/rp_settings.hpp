#pragma once

#include <cstddef>

//------------------------------------------------------------------------------

namespace reporter {

//------------------------------------------------------------------------------

class Settings
{
public:
	virtual ~Settings() = default;

	using CountType = std::size_t;

	virtual CountType getMaxFilesCount() const = 0;
	virtual void setMaxFilesCount( CountType _count ) = 0;

	virtual CountType getMaxDetailsCount() const = 0;
	virtual void setMaxDetailsCount( CountType _count ) = 0;

	virtual bool getShowStdFiles() const = 0;
	virtual void setShowStdFiles( bool _enable ) = 0;

	virtual bool getShowOnlyStdHeaders() const = 0;
	virtual void setShowOnlyStdHeaders( bool _enable ) = 0;

	virtual void copy( const Settings & _other ) = 0;
};

//------------------------------------------------------------------------------

}
