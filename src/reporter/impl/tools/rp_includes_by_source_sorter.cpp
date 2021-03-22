#include "reporter/impl/tools/rp_includes_by_source_sorter.hpp"

#include "reporter/impl/tools/rp_file_sorter.hpp"

#include "model_includes/api/mi_include.hpp"

#include "exception/ih/exc_internal_error.hpp"

//------------------------------------------------------------------------------

namespace reporter {

//------------------------------------------------------------------------------

bool IncludesBySourceSorter::operator()(
	const Include * _l,
	const Include * _r
) const
{
	INTERNAL_CHECK_WARRING( _l );
	INTERNAL_CHECK_WARRING( _r );

	if( _l == nullptr || _r == nullptr )
	{
		return false;
	}

	return operator()( *_l, *_r );
}

//------------------------------------------------------------------------------

bool IncludesBySourceSorter::operator()(
	const Include & _l,
	const Include & _r
) const
{
	return FileSorter()( _l.getSourceFile(), _r.getSourceFile() );
}

//------------------------------------------------------------------------------

}
