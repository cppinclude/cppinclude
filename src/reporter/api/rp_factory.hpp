#pragma once

#include <stdfwd.hpp>

//------------------------------------------------------------------------------

namespace reporter {
	class Reporter;
	class Settings;
	enum class ReporterKind;

//------------------------------------------------------------------------------

class Factory
{
public:
	using ReporterPtr = stdfwd::unique_ptr< Reporter >;
	using SettingsPtr = stdfwd::unique_ptr< Settings >;

	virtual ~Factory() = default;

	virtual ReporterPtr createReporter( ReporterKind _kind ) = 0;

	virtual ReporterPtr createDumpReporter() = 0;
	virtual ReporterPtr createUnresolvedReporter() = 0;
	virtual ReporterPtr createMostImpactReporter() = 0;
	virtual ReporterPtr createUnincludedReporter() = 0;
	virtual ReporterPtr createDifferentTypeReporter() = 0;

	virtual SettingsPtr createSettings() = 0;
};

//------------------------------------------------------------------------------

}
