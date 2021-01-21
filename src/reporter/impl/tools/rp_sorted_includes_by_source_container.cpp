#include "reporter/impl/tools/rp_sorted_includes_by_source_container.hpp"

#include "model_includes/api/mi_include.hpp"

#include "exception/ih/exc_internal_error.hpp"

#include <functional>

//------------------------------------------------------------------------------

namespace reporter {

//------------------------------------------------------------------------------

void SortedIncludesBySourceContainer::insert( const Include & _include )
{
	m_includes.insert( &_include );
}

//------------------------------------------------------------------------------

void SortedIncludesBySourceContainer::forEachInclude(
	IncludeCallback _callback
) const
{
	for( const Include * include : m_includes )
	{
		INTERNAL_CHECK_WARRING( include );
		if( !include )
			continue;

		if( !_callback( *include ) )
			break;
	}
}

//------------------------------------------------------------------------------

void SortedIncludesBySourceContainer::forEachSource( FileCallback _callback ) const
{
	forEachInclude(
		[&]( const Include & _include )
		{
			return _callback( _include.getSourceFile() );
		}
	);
}

//------------------------------------------------------------------------------

std::size_t SortedIncludesBySourceContainer::getCount() const
{
	return m_includes.size();
}

//------------------------------------------------------------------------------

}
