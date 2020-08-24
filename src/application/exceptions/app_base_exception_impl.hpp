#pragma once

#include "exception/ih/exc_exception_impl.hpp"

#include "application/resources/app_resources_exceptions.hpp"

//------------------------------------------------------------------------------

namespace application {

//------------------------------------------------------------------------------

template< class _BaseException >
class BaseExceptionImpl : public ::exception::ExceptionImpl< _BaseException >
{
public:

    using BaseClass = ::exception::ExceptionImpl< _BaseException >;

	explicit BaseExceptionImpl( std::string_view _code )
		:	BaseClass{ resources::exceptions::ModuleName, _code }
	{

	}

};

//------------------------------------------------------------------------------

}
