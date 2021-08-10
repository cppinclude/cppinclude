#pragma once

#include "project/resources/prj_resources_exceptions.hpp"

#include "exception/ih/exc_exception_impl.hpp"

//------------------------------------------------------------------------------

namespace project
{
//------------------------------------------------------------------------------

template< class _BaseException >
class BaseExceptionImpl : public ::exception::ExceptionImpl< _BaseException >
{
	using BaseClass = ::exception::ExceptionImpl< _BaseException >;

public:
	explicit BaseExceptionImpl( std::string_view _code )
		: BaseClass( resources::exceptions::ModuleName, _code )
	{
	}
};

//------------------------------------------------------------------------------

}
