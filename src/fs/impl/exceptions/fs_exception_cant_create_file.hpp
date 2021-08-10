#pragma once

#include "fs/api/fs_exceptions.hpp"
#include "fs/impl/exceptions/fs_base_exception_impl.hpp"

#include <std_fs>

//------------------------------------------------------------------------------

namespace fs
{
//------------------------------------------------------------------------------

class CantCreateFileImpl : public BaseExceptionImpl< CantCreateFile >
{
	using BaseClass = BaseExceptionImpl< CantCreateFile >;

public:
	using Path = stdfs::path;

	explicit CantCreateFileImpl( Path _path );

	std::string getMessage() const noexcept override;

private:
	const Path m_path;
};

//------------------------------------------------------------------------------

}
