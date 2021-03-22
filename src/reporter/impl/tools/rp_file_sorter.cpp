#include "reporter/impl/tools/rp_file_sorter.hpp"

#include "model_includes/api/mi_file.hpp"

#include "exception/ih/exc_internal_error.hpp"

#include <std_fs>

//------------------------------------------------------------------------------

namespace reporter {

//------------------------------------------------------------------------------

bool FileSorter::operator()( const File * _l, const File * _r ) const
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

bool FileSorter::operator()( const File & _l, const File & _r ) const
{
	const auto & lPath = _l.getPath();
	const auto & rPath = _r.getPath();

	return stdfs::less( lPath, rPath );
}

//------------------------------------------------------------------------------

}
