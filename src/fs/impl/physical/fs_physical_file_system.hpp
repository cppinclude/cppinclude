#pragma once

#include "fs/api/fs_file_system.hpp"

//------------------------------------------------------------------------------

namespace fs::physical
{
//------------------------------------------------------------------------------

class PhysicalFileSystem final : public FileSystem
{
public:
	FilePtr openFile( const Path & _path ) const override;
	FilePtr createFile( const Path & _path ) override;
	bool isExistFile( const Path & _path ) const override;
	Path getCurrentPath() const override;

	void
	forEachItem( const Path & _ditPath, ItemCallback _callback ) const override;

	Path toAbsolute( const Path & _path ) const override;

private:
	FilePtr getFile( const Path & _path, bool _create ) const;
};

//------------------------------------------------------------------------------

}
