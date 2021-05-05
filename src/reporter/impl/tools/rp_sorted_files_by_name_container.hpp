#pragma once

#include "reporter/impl/tools/rp_file_sorter.hpp"

#include <stdfwd/functional>
#include <std_fs>

#include <set>

//------------------------------------------------------------------------------

namespace model_includes {
	class File;
}

//------------------------------------------------------------------------------

namespace reporter {

//------------------------------------------------------------------------------

class SortedFilesByNameContainer
{
public:

	using File			= model_includes::File;
	using FileCallback	= stdfwd::function< bool ( const File & ) >;

	SortedFilesByNameContainer();

	void insert( const File & _file );
	bool isEmpty() const;

	void forEachFile( const FileCallback & _callback ) const;

	std::size_t getSize() const;

private:
	using Path = stdfs::path;

	using FilesContainer = std::set< const File *, FileSorter >;

	FilesContainer m_files;
};

//------------------------------------------------------------------------------

}
