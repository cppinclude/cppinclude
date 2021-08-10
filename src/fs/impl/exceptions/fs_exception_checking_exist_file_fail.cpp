#include "fs/impl/exceptions/fs_exception_checking_exist_file_fail.hpp"

#include "fs/resources/fs_resources_exceptions.hpp"

//------------------------------------------------------------------------------

namespace fs
{
//------------------------------------------------------------------------------

CheckingExistFileFailImpl::CheckingExistFileFailImpl(
	Path _path, const std::error_code & _code )
	: BaseClass( resources::exceptions::CheckingExistFileFail::Code )
	, m_path{ std::move( _path ) }
	, m_originMsg{ _code.message() }
{
}

//------------------------------------------------------------------------------

std::string CheckingExistFileFailImpl::getMessage() const noexcept
{
	using namespace resources;
	return exceptions::CheckingExistFileFail::Msg + m_path.string() + " ." +
		   m_originMsg;
}

//------------------------------------------------------------------------------

}
