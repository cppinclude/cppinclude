#include "reporter/impl/tools/rp_file_with_count_sorter.hpp"

#include "reporter/impl/tools/rp_file_sorter.hpp"
#include "reporter/impl/tools/rp_file_with_count.hpp"

//------------------------------------------------------------------------------

namespace reporter
{
//------------------------------------------------------------------------------

bool FileWithCountSorter::operator()(
	const FileWithCount & _l, const FileWithCount & _r ) const
{
	if( _l.getCount() != _r.getCount() )
	{
		// max is first
		return _l.getCount() > _r.getCount();
	}

	return FileSorter()( _l.getFile(), _r.getFile() );
}

//------------------------------------------------------------------------------

}
