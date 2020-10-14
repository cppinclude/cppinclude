#include "fs/impl/physical/fs_physical_file_system.hpp"

#include "fs/impl/physical/fs_physical_file.hpp"
#include "fs/api/enums/fs_item_type.hpp"

#include "fs/impl/exceptions/fs_exception_cant_open_file_impl.hpp"
#include "fs/impl/exceptions/fs_exception_cant_create_file.hpp"

#include <memory>
#include <functional>

//------------------------------------------------------------------------------

namespace fs::physical {

//------------------------------------------------------------------------------

PhysicalFileSystem::FilePtr PhysicalFileSystem::openFile(
	const Path & _path
) const
{
	const bool create = false;
	auto filePtr = getFile( _path, create );
	if( !filePtr )
	{
		throw CantOpenFileImpl( _path );
	}
	return filePtr;
}

//------------------------------------------------------------------------------

PhysicalFileSystem::FilePtr PhysicalFileSystem::createFile(
	const Path & _path
)
{
	const bool create = true;
	auto filePtr = getFile( _path, create );
	if( !filePtr )
	{
		throw CantCreateFileImpl( _path );
	}
	return filePtr;
}

//------------------------------------------------------------------------------

bool PhysicalFileSystem::isExistFile( const Path & _path ) const
{
	return stdfs::exists( _path );
}

//------------------------------------------------------------------------------

PhysicalFileSystem::Path PhysicalFileSystem::toAbsolute( const Path & _path ) const
{
	return stdfs::absolute( _path );
}

//------------------------------------------------------------------------------

PhysicalFileSystem::FilePtr PhysicalFileSystem::getFile(
	const Path & _path,
	bool _create
) const
{
	const std::ios_base::openmode mode =
		_create ? std::ios_base::out : std::ios_base::in;

	std::fstream file;
	file.open( _path, mode );
	if( file )
	{
		return FilePtr{ new PhysicalFile{ std::move( file ) } };
	}

	return nullptr;
}

//------------------------------------------------------------------------------

void PhysicalFileSystem::forEachItem(
	const Path & _ditPath,
	ItemCallback _callback
) const
{
	for(auto& item : stdfs::directory_iterator( _ditPath ) )
	{
		static_assert( static_cast< int >( ItemType::Count ) == 2 );
		ItemType type =
			stdfs::is_directory( item ) ?
			ItemType::Folder :
			ItemType::File
		;
		_callback( item, type );
	}
}

//------------------------------------------------------------------------------

PhysicalFileSystem::Path PhysicalFileSystem::getCurrentPath() const
{
	return stdfs::current_path();
}

//------------------------------------------------------------------------------

}
