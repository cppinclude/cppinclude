#pragma once

#include "reporter/impl/tools/rp_file_with_count.hpp"
#include "reporter/impl/tools/rp_file_with_count_sorter.hpp"

#include <set>
#include <stdfwd.hpp>

//------------------------------------------------------------------------------

namespace model_includes {
	class File;
}

//------------------------------------------------------------------------------

namespace reporter {

//------------------------------------------------------------------------------

class FileWithCountContainer
{
public:

	using File				= model_includes::File;
	using Files				= std::set< FileWithCount, FileWithCountSorter >;
	using FileCallback		= stdfwd::function< bool ( const File & ) >;
	using Iterator			= Files::iterator;

	std::size_t getSize() const;
	bool isEmpty() const;

	void insert( const FileWithCount & _file );
	void resizeByLimit( std::size_t _limit );

	void forEachFile( const FileCallback & _callback ) const;

	Iterator begin() const;
	Iterator end() const;

private:
	Files m_files;
};

//------------------------------------------------------------------------------

}
