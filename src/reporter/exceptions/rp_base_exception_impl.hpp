#pragma once

#include "exception/ih/exc_exception_impl.hpp"

#include "reporter/resources/rp_resources_exceptions.hpp"

//------------------------------------------------------------------------------

namespace reporter {

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
