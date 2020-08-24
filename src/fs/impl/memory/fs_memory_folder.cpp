#include "fs/impl/memory/fs_memory_folder.hpp"

#include "fs/impl/memory/fs_memory_file.hpp"

//------------------------------------------------------------------------------

namespace fs::memory {

//------------------------------------------------------------------------------

MemoryFolder::MemoryFolder( std::string_view _name )
	:	m_name{ _name }
{

}

//------------------------------------------------------------------------------

MemoryFolder::FolderPtr MemoryFolder::ensureSubFolder( std::string_view _name )
{
	std::string name{ _name };
	auto pair = m_subdirs.try_emplace( name, FolderPtr{ new MemoryFolder{ _name } } );
	auto it = pair.first;
	FolderPtr & folderPtr = it->second;
	return folderPtr;
}

//------------------------------------------------------------------------------

MemoryFolder::FolderPtr MemoryFolder::getSubFolder( std::string_view _name ) const
{
	const std::string name{ _name };
	if( auto it = m_subdirs.find( name ); it != m_subdirs.end() )
		return it->second;

	return nullptr;
}

//------------------------------------------------------------------------------

MemoryFolder::FilePtr MemoryFolder::ensureFile( std::string_view _name )
{
	std::string name{ _name };
	auto pair = m_files.try_emplace( name, FilePtr{ new MemoryFile{} } );
	auto it = pair.first;
	FilePtr & filePtr = it->second;
	return filePtr;
}

//------------------------------------------------------------------------------

MemoryFolder::FilePtr MemoryFolder::getFile( std::string_view _name ) const
{
	const std::string name{ _name };
	if( auto it = m_files.find( name ); it != m_files.end() )
		return it->second;

	return nullptr;
}

//------------------------------------------------------------------------------

void MemoryFolder::forEachItem( ItemCallback _callback )
{
	for( auto it : m_subdirs )
		_callback( it.first, ItemType::Folder );

	for( auto it : m_files )
		_callback( it.first, ItemType::File );
}

//------------------------------------------------------------------------------

}
