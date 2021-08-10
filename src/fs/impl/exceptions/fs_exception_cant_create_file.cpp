#include "fs/impl/exceptions/fs_exception_cant_create_file.hpp"

#include "fs/resources/fs_resources_exceptions.hpp"

#include <string>

//------------------------------------------------------------------------------

namespace fs
{
//------------------------------------------------------------------------------

CantCreateFileImpl::CantCreateFileImpl( Path _path )
	: BaseClass( resources::exceptions::CantCreateFile::Code )
	, m_path{ std::move( _path ) }
{
}

//------------------------------------------------------------------------------

std::string CantCreateFileImpl::getMessage() const noexcept
{
	return resources::exceptions::CantCreateFile::Msg + m_path.string();
}

//------------------------------------------------------------------------------

}
