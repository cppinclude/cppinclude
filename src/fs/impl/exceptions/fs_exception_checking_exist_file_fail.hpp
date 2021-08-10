#pragma once

#include "fs/api/fs_exceptions.hpp"
#include "fs/impl/exceptions/fs_base_exception_impl.hpp"

#include <std_fs>

#include <system_error>

//------------------------------------------------------------------------------

namespace fs
{
//------------------------------------------------------------------------------

class CheckingExistFileFailImpl
	: public BaseExceptionImpl< CheckingExistFileFail >
{
	using BaseClass = BaseExceptionImpl< CheckingExistFileFail >;

public:
	using Path = stdfs::path;

	CheckingExistFileFailImpl( Path _path, const std::error_code & _code );

	std::string getMessage() const noexcept override;

private:
	const Path m_path;
	const std::string m_originMsg;
};

//------------------------------------------------------------------------------

}
