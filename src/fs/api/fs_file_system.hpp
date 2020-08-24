#pragma once

#include <stdfwd.hpp>

//------------------------------------------------------------------------------

namespace fs {

class File;
enum class ItemType;

//------------------------------------------------------------------------------

class FileSystem
{
public:

	using Path			= std::filesystem::path;
	using FilePtr		= stdfwd::shared_ptr< File >;
	using ItemCallback	= std::function< void ( const Path &, ItemType ) >;

	virtual ~FileSystem() = default;

	virtual FilePtr openFile( const Path & _path ) const = 0;
	virtual FilePtr createFile( const Path & _path ) = 0;
	virtual bool isExistFile( const Path & _path ) const = 0;

	virtual Path getCurrentPath() const = 0;
	virtual Path toAbsolute( const Path & _path ) const = 0;

	virtual void forEachItem(
		const Path & _dirPath,
		ItemCallback _callback
	) const = 0;

};

//------------------------------------------------------------------------------

}
