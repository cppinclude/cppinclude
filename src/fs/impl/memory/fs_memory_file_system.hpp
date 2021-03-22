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
		const Path & _dirPath,
		ItemCallback _callback
	) const override;

	Path getCurrentPath() const override;
	Path toAbsolute( const Path & _path ) const override;

private:
	using FolderPtr = std::shared_ptr< MemoryFolder >;

	FolderPtr getRoot( const Path & _path ) const;
	MemoryFolder & ensureRoot( const Path & _path );

	FolderPtr getFolder( const Path & _path ) const;
	MemoryFolder & ensureFolder( const Path & _path );

	Path toAbsolutePath( const Path & _path ) const;

private:
	using Roots = std::unordered_map< Path, FolderPtr >;
	Roots m_roots;
};

//------------------------------------------------------------------------------

}
