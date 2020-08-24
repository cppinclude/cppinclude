#pragma once

#include "fs/api/exceptions/fs_exceptions.hpp"
#include "fs/impl/exceptions/fs_base_exception_impl.hpp"

#include <filesystem>

//------------------------------------------------------------------------------

namespace fs {

//------------------------------------------------------------------------------

class CantOpenFileImpl : public BaseExceptionImpl< CantOpenFile >
{
	using BaseClass = BaseExceptionImpl< CantOpenFile >;

public:

	explicit CantOpenFileImpl( const std::filesystem::path & _path );

	std::string getMessage() const noexcept override;

private:

	const std::filesystem::path m_path;
};

//------------------------------------------------------------------------------

}
