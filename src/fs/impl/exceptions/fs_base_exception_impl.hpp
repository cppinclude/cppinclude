#pragma once

#include "fs/resources/fs_resources_exceptions.hpp"

#include "exception/ih/exc_exception_impl.hpp"

//------------------------------------------------------------------------------

namespace fs {

//------------------------------------------------------------------------------

template< class _BaseException >
class BaseExceptionImpl : public ::exception::ExceptionImpl< _BaseException >
{
	using BaseClass = ::exception::ExceptionImpl< _BaseException >;

public:
	explicit BaseExceptionImpl( std::string_view _code )
		:	BaseClass( resources::exceptions::ModuleName, _code )
	{
	}
};

//------------------------------------------------------------------------------

}
