#pragma once

#include "reporter/impl/tools/rp_sorted_includes_by_source_container.hpp"

//------------------------------------------------------------------------------

namespace reporter {

//------------------------------------------------------------------------------

class DifferentTypeReporterDetails
{
public:

	using Include	= model_includes::Include;
	using Includes	= SortedIncludesBySourceContainer;

	const Includes & getUserIncludes() const;
	const Includes & getSystemIncludes() const;

	void insertInclude( const Include & _include );

private:
	Includes m_userIncludes;
	Includes m_systemIncludes;
};

//------------------------------------------------------------------------------

}
