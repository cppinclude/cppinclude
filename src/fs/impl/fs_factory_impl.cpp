#include "fs/impl/fs_factory_impl.hpp"

#include "fs/impl/memory/fs_memory_file_system.hpp"
#include "fs/impl/physical/fs_physical_file_system.hpp"

//------------------------------------------------------------------------------

namespace fs {

//------------------------------------------------------------------------------

FactoryImpl::~FactoryImpl() = default;

//------------------------------------------------------------------------------

FileSystem & FactoryImpl::getPhysicalFileSystem()
{
	if( !m_physicalFs )
		m_physicalFs.reset( new physical::PhysicalFileSystem );

	return *m_physicalFs;
}

//------------------------------------------------------------------------------

FileSystem & FactoryImpl::getMemoryFileSystem()
{
	if( !m_memoryFs )
		m_memoryFs.reset( new memory::MemoryFileSystem );

	return *m_memoryFs;
}

//------------------------------------------------------------------------------

}
