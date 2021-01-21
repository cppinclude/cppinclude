#pragma once

//------------------------------------------------------------------------------

namespace model_includes {
	class Include;
}

//------------------------------------------------------------------------------

namespace reporter {

//------------------------------------------------------------------------------

struct IncludesBySourceSorter
{
	using Include = model_includes::Include;

	bool operator()( const Include * _l, const Include * _r ) const;
	bool operator()( const Include & _l, const Include & _r ) const;
};

//------------------------------------------------------------------------------

}
