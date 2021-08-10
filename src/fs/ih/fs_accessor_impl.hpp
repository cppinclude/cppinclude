#pragma once

#include "fs/ih/fs_accessor.hpp"

#include <memory>

//------------------------------------------------------------------------------

namespace fs
{
//------------------------------------------------------------------------------

class FileSystemAccessorImpl final : public FileSystemAccessor
{
public:
	FileSystemAccessorImpl();
	~FileSystemAccessorImpl() override;

	Factory & getFactory() override;

private:
	std::unique_ptr< Factory > m_factoryPtr;
};

//------------------------------------------------------------------------------

}

//------------------------------------------------------------------------------
