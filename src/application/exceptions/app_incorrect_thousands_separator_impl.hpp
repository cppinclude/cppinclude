#pragma once

#include "application/exceptions/app_base_exception_with_message_impl.hpp"
#include "application/exceptions/app_exceptions.hpp"

//------------------------------------------------------------------------------

namespace application
{
//------------------------------------------------------------------------------

class IncorrectThousandsSeparatorImpl
	: public BaseExceptionWithMessageImpl< IncorrectThousandsSeparator >
{
	using BaseClass =
		BaseExceptionWithMessageImpl< IncorrectThousandsSeparator >;

public:
	IncorrectThousandsSeparatorImpl( const std::string & _separator );
};

//------------------------------------------------------------------------------

}
