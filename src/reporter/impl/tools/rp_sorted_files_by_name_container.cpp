#include "reporter/impl/tools/rp_sorted_files_by_name_container.hpp"

#include "model_includes/api/mi_file.hpp"

#include "exception/ih/exc_internal_error.hpp"

#include <std_fs>

#include <functional>

//------------------------------------------------------------------------------

namespace reporter
{
//------------------------------------------------------------------------------

SortedFilesByNameContainer::SortedFilesByNameContainer() = default;

//------------------------------------------------------------------------------

void SortedFilesByNameContainer::insert( const model_includes::File & _file )
{
	m_files.insert( &_file );
}

//------------------------------------------------------------------------------

bool SortedFilesByNameContainer::isEmpty() const
{
	return m_files.empty();
}

//------------------------------------------------------------------------------

void SortedFilesByNameContainer::forEachFile(
	const FileCallback & _callback ) const
{
	for( const File * filePtr: m_files )
	{
		if( filePtr == nullptr )
		{
			INTERNAL_CHECK_WARRING( false );
			continue;
		}

		if( !_callback( *filePtr ) )
		{
			break;
		}
	}
}

//------------------------------------------------------------------------------

std::size_t SortedFilesByNameContainer::getSize() const
{
	return m_files.size();
}

//------------------------------------------------------------------------------

}
