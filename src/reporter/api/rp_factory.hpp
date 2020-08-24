#pragma once

#include <stdfwd.hpp>

//------------------------------------------------------------------------------

namespace reporter {
	class Reporter;
	enum class ReporterKind;

//------------------------------------------------------------------------------

class Factory
{
public:

	using ReporterPtr = stdfwd::unique_ptr< Reporter >;

	virtual ~Factory() = default;

	virtual ReporterPtr createReporter( ReporterKind _kind ) = 0;

	virtual ReporterPtr createDumpReporter() = 0;
	virtual ReporterPtr createUnresolvedReporter() = 0;
	virtual ReporterPtr createMostImpactReporter() = 0;
};

//------------------------------------------------------------------------------

}
