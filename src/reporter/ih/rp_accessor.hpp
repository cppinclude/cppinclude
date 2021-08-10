#pragma once

//------------------------------------------------------------------------------

namespace reporter
{
class Factory;

//------------------------------------------------------------------------------

class ReporterAccessor
{
public:
	virtual ~ReporterAccessor() = default;

	virtual Factory & getReporterFactory() = 0;
};

//------------------------------------------------------------------------------

}
