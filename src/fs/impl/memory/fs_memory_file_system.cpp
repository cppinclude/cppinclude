#include "fs/impl/memory/fs_memory_file_system.hpp"

#include "fs/impl/memory/fs_memory_file.hpp"
#include "fs/impl/memory/fs_memory_folder.hpp"

#include "exception/ih/exc_internal_error.hpp"

#include <memory>
#include <filesystem>

//------------------------------------------------------------------------------

namespace fs::memory {

//------------------------------------------------------------------------------

MemoryFileSystem::FilePtr MemoryFileSystem::openFile(
	const std::filesystem::path & _path
) const
{
	const Path path = toAbsolutePath( _path );

	FolderPtr dirPtr = getFolder( path.parent_path() );
	if( !dirPtr )
		return nullptr;

	const std::filesystem::path fileName = _path.filename();
	return dirPtr->getFile( fileName.string() );
}

//------------------------------------------------------------------------------

MemoryFileSystem::FilePtr MemoryFileSystem::createFile(
	const std::filesystem::path & _path
)
{
	const Path path = toAbsolutePath( _path );
	MemoryFolder & folder = ensureFolder( path.parent_path() );

	const std::filesystem::path fileName = _path.filename();
	return folder.ensureFile( fileName.string() );
}

//------------------------------------------------------------------------------

bool MemoryFileSystem::isExistFile( const std::filesystem::path & _path ) const
{
	return openFile( _path ).get();
}

//------------------------------------------------------------------------------

MemoryFileSystem::Path MemoryFileSystem::getCurrentPath() const
{
	return "/tmp/";
}

//------------------------------------------------------------------------------

MemoryFileSystem::Path MemoryFileSystem::toAbsolute( const Path & _path ) const
{
	return getCurrentPath() / _path;
}

//------------------------------------------------------------------------------

void MemoryFileSystem::forEachItem(
	const std::filesystem::path & _dirPath,
	ItemCallback _callback
) const
{
	FolderPtr folderPtr = getFolder( _dirPath );
	INTERNAL_CHECK_WARRING( folderPtr );
	if( !folderPtr )
		return;

	MemoryFolder & folder = *folderPtr;
	folder.forEachItem(
		[&]( std::string_view _name, ItemType _type )
		{
			std::filesystem::path path = _dirPath / _name;
			_callback( path, _type );
		}
	);
}

//------------------------------------------------------------------------------

MemoryFolder & MemoryFileSystem::ensureRoot( const std::filesystem::path & _path )
{
	auto pair = m_roots.try_emplace(
		_path,
		FolderPtr{ new MemoryFolder{ _path.string() } }
	);
	auto it = pair.first;
	FolderPtr & rootPtr = it->second;
	INTERNAL_CHECK_ERROR( rootPtr );
	return *rootPtr;
}

//------------------------------------------------------------------------------

MemoryFileSystem::FolderPtr MemoryFileSystem::getRoot(
	const std::filesystem::path & _path
) const
{
	if( auto it = m_roots.find( _path ); it != m_roots.end() )
		return it->second;

	return nullptr;
}

//------------------------------------------------------------------------------

MemoryFileSystem::FolderPtr MemoryFileSystem::getFolder(
	const std::filesystem::path & _path
) const
{
	FolderPtr rootPtr = getRoot( _path.root_directory() );
	if( !rootPtr )
		return nullptr;

	FolderPtr currentFolder = rootPtr;
	const std::filesystem::path pathWitoutRoot = _path.relative_path();
	for( auto currentName : pathWitoutRoot )
	{
		const std::string folderName = currentName.string();
		if( folderName.empty() )
			continue;

		INTERNAL_CHECK_ERROR( currentFolder );
		auto subDirPtr = currentFolder->getSubFolder( currentName.string() );

		currentFolder = subDirPtr;
		if( !currentFolder )
			break;
	}
	return currentFolder;
}

//------------------------------------------------------------------------------

MemoryFolder & MemoryFileSystem::ensureFolder(
	const std::filesystem::path & _path
)
{
	Path pathWitoutRoot = _path.relative_path();
	MemoryFolder & root = ensureRoot( _path.root_directory() );
	MemoryFolder * currentFolder = &root;
	for( auto currentName : pathWitoutRoot )
	{
		const std::string folderName = currentName.string();
		if( folderName.empty() )
			continue;

		INTERNAL_CHECK_ERROR( currentFolder );
		auto subDirPtr = currentFolder->ensureSubFolder( currentName.string() );
		currentFolder = subDirPtr.get();
	}

	INTERNAL_CHECK_ERROR( currentFolder );
	return *currentFolder;
}

//------------------------------------------------------------------------------

MemoryFileSystem::Path MemoryFileSystem::toAbsolutePath(
	const Path & _path
) const
{
	if( !_path.is_absolute() )
		return getCurrentPath() / _path;

	return _path;
}

//------------------------------------------------------------------------------

}
