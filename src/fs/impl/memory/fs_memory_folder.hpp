#pragma once

#include "fs/api/enums/fs_item_type.hpp"

#include <string>
#include <unordered_map>
#include <memory>
#include <functional>

//------------------------------------------------------------------------------

namespace fs::memory {

class MemoryFile;

//------------------------------------------------------------------------------

class MemoryFolder
{
public:

	using FolderPtr		= std::shared_ptr< MemoryFolder >;
	using FilePtr		= std::shared_ptr< MemoryFile >;
	using ItemCallback  = std::function< void ( std::string_view , ItemType ) > ;

	MemoryFolder( std::string_view _name );

	FolderPtr ensureSubFolder( std::string_view _name );
	FolderPtr getSubFolder( std::string_view _name ) const;

	FilePtr ensureFile( std::string_view _name );
	FilePtr getFile( std::string_view _name ) const;

	void forEachItem( ItemCallback _callback );

private:

	using SubFolders	= std::unordered_map< std::string, FolderPtr >;
	using Files			= std::unordered_map< std::string, FilePtr >;

	SubFolders m_subdirs;
	Files m_files;

	const std::string m_name;
};

//------------------------------------------------------------------------------

}

//------------------------------------------------------------------------------
