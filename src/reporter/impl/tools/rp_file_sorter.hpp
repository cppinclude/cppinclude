#pragma once

//------------------------------------------------------------------------------

namespace model_includes {
	class File;
}

//------------------------------------------------------------------------------

namespace reporter {

//------------------------------------------------------------------------------

struct FileSorter
{
	using File = model_includes::File;

	bool operator()( const File * _l, const File * _r ) const;
	bool operator()( const File & _l, const File & _r ) const;
};

//------------------------------------------------------------------------------

}
