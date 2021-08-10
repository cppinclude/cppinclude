#pragma once

//------------------------------------------------------------------------------

namespace fs
{
class FileSystem;

//------------------------------------------------------------------------------

class Factory
{
public:
	virtual ~Factory() = default;

	virtual FileSystem & getPhysicalFileSystem() = 0;
	virtual FileSystem & getMemoryFileSystem() = 0;
};

//------------------------------------------------------------------------------

}

//------------------------------------------------------------------------------
