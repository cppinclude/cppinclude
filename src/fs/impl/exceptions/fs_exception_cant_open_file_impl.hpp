#pragma once

#include "fs/api/exceptions/fs_exceptions.hpp"
#include "fs/impl/exceptions/fs_base_exception_impl.hpp"

#include <std_fs>

//------------------------------------------------------------------------------

namespace fs {

//------------------------------------------------------------------------------

class CantOpenFileImpl : public BaseExceptionImpl< CantOpenFile >
{
	using BaseClass = BaseExceptionImpl< CantOpenFile >;

public:

	using Path = stdfs::path;

	explicit CantOpenFileImpl( const Path & _path );

	std::string getMessage() const noexcept override;

private:

	const Path m_path;
};

//------------------------------------------------------------------------------

}
