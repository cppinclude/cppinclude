#include "reporter/impl/tools/rp_file_with_count_container.hpp"

#include "model_includes/api/mi_file.hpp"

#include <functional>

//------------------------------------------------------------------------------

namespace reporter {

//------------------------------------------------------------------------------

void FileWithCountContainer::insert( const FileWithCount & _file )
{
	m_files.insert( _file );
}

//------------------------------------------------------------------------------

std::size_t FileWithCountContainer::getSize() const
{
	return m_files.size();
}

//------------------------------------------------------------------------------

bool FileWithCountContainer::isEmpty() const
{
	return m_files.empty();
}

//------------------------------------------------------------------------------

void FileWithCountContainer::resizeByLimit( std::size_t _limit )
{
	if( _limit > 0 && m_files.size() >= _limit )
	{
		auto it = m_files.begin();
		for( std::size_t i = 0; i < _limit; ++i )
		{
			++it;
		}

		m_files.erase( it, m_files.end() );
	}
}

//------------------------------------------------------------------------------

void FileWithCountContainer::forEachFile( const FileCallback & _callback ) const
{
	for( const FileWithCount & fileInfo: m_files )
	{
		const File & file = fileInfo.getFile();
		if( !_callback( file ) )
		{
			break;
		}
	}
}

//------------------------------------------------------------------------------

FileWithCountContainer::Iterator FileWithCountContainer::begin() const
{
	return m_files.begin();
}

//------------------------------------------------------------------------------

FileWithCountContainer::Iterator FileWithCountContainer::end() const
{
	return m_files.end();
}

//------------------------------------------------------------------------------

}
