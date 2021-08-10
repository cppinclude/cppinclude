#pragma once

//------------------------------------------------------------------------------

namespace reporter
{
class FileWithCount;

//------------------------------------------------------------------------------

struct FileWithCountSorter
{
	bool operator()( const FileWithCount & _l, const FileWithCount & _r ) const;
};

//------------------------------------------------------------------------------

}
