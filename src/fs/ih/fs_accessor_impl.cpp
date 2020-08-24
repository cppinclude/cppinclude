#include "fs/ih/fs_accessor_impl.hpp"

#include "fs/impl/fs_factory_impl.hpp"

#include "exception/ih/exc_internal_error.hpp"

//------------------------------------------------------------------------------

namespace fs {

//------------------------------------------------------------------------------

FileSystemAccessorImpl::FileSystemAccessorImpl()
	:	m_factoryPtr{ new FactoryImpl }
{

}

//------------------------------------------------------------------------------

FileSystemAccessorImpl::~FileSystemAccessorImpl()
{

};

//------------------------------------------------------------------------------

Factory & FileSystemAccessorImpl::getFactory()
{
	INTERNAL_CHECK_ERROR( m_factoryPtr );
	return *m_factoryPtr;
}

//------------------------------------------------------------------------------

}
