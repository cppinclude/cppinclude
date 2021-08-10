#pragma once

#include "fs/api/fs_factory.hpp"
#include "fs/api/fs_file_system.hpp"

#include <memory>

//------------------------------------------------------------------------------

namespace fs
{
//------------------------------------------------------------------------------

class FactoryImpl final : public Factory
{
public:
	~FactoryImpl() override;

	FileSystem & getPhysicalFileSystem() override;
	FileSystem & getMemoryFileSystem() override;

private:
	using FsPtr = std::unique_ptr< FileSystem >;

	FsPtr m_physicalFs;
	FsPtr m_memoryFs;
};

//------------------------------------------------------------------------------

}
