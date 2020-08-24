#pragma once

#include "fs/api/fs_file_system.hpp"
#include "tools/std_hash_fs_path.hpp"

#include <unordered_map>

//------------------------------------------------------------------------------

namespace fs::memory {

class MemoryFolder;

//------------------------------------------------------------------------------

class MemoryFileSystem final : public FileSystem
{
public:

	FilePtr openFile( const Path & _path ) const override;
	FilePtr createFile( const Path & _path ) override;
	bool isExistFile( const Path & _path ) const override;

	void forEachItem(
		const Path & _ditPath,
		ItemCallback _callback
	) const override;

	Path getCurrentPath() const override;
	Path toAbsolute( const Path & _path ) const override;

private:

	using FolderPtr = std::shared_ptr< MemoryFolder >;

	FolderPtr getRoot( const std::filesystem::path & _path ) const;
	MemoryFolder & ensureRoot( const std::filesystem::path & _path );

	FolderPtr getFolder( const std::filesystem::path & _path ) const;
	MemoryFolder & ensureFolder( const std::filesystem::path & _path );

	Path toAbsolutePath( const Path & _path ) const;

private:

	using Roots = std::unordered_map< std::filesystem::path, FolderPtr >;
	Roots m_roots;
};

//------------------------------------------------------------------------------

}
