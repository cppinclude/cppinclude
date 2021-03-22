#pragma once

#include "application/resources/app_resources_exceptions.hpp"

#include "exception/ih/exc_exception_with_message_impl.hpp"

//------------------------------------------------------------------------------

namespace application
{
//------------------------------------------------------------------------------

template< class _BaseException >
class BaseExceptionWithMessageImpl : public
	::exception::ExceptionWithMessageImpl< _BaseException >
{
public:
	using BaseClass = ::exception::ExceptionWithMessageImpl< _BaseException >;

	BaseExceptionWithMessageImpl(
			std::string_view _code,
			std::string_view _message
	)
		:	BaseClass{ resources::exceptions::ModuleName, _code, _message }
	{
	}
};

//------------------------------------------------------------------------------

}
