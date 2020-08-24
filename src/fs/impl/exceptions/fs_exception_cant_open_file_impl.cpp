#include "fs/impl/exceptions/fs_exception_cant_open_file_impl.hpp"

#include "fs/resources/fs_resources_exceptions.hpp"

#include <string>

//------------------------------------------------------------------------------

namespace fs {

//------------------------------------------------------------------------------

CantOpenFileImpl::CantOpenFileImpl( const std::filesystem::path & _path )
	:	BaseClass( resources::exceptions::CantOpenFile::Code )
	,	m_path{ _path }
{

}

//------------------------------------------------------------------------------

std::string CantOpenFileImpl::getMessage() const noexcept
{
	return resources::exceptions::CantOpenFile::Msg + m_path.string();
}

//------------------------------------------------------------------------------

}
