#pragma once

#include <iosfwd>

#include <stdio.h>

//------------------------------------------------------------------------------

namespace model_includes {
	class Model;
}

//------------------------------------------------------------------------------

namespace reporter {
	class Settings;
	enum class ReporterKind;

//------------------------------------------------------------------------------

class Reporter
{
public:
	virtual ~Reporter() = default;

	virtual ReporterKind getKind() const = 0;

	virtual const Settings & getSettings() const = 0;
	virtual void copySettings( const Settings & _settings ) = 0;

	virtual void report(
		const model_includes::Model & _model,
		std::ostream & _stream
	) = 0;
};

//------------------------------------------------------------------------------

}
