#pragma once

#include "application/exceptions/app_exceptions.hpp"
#include "application/exceptions/app_base_exception_with_message_impl.hpp"

//------------------------------------------------------------------------------

namespace application {

//------------------------------------------------------------------------------

class CantCreateParserImpl :
	public BaseExceptionWithMessageImpl< CantCreateParser >
{
	using BaseClass = BaseExceptionWithMessageImpl< CantCreateParser >;

public:

	CantCreateParserImpl();
};

//------------------------------------------------------------------------------

}
