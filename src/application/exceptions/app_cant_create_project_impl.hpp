#pragma once

#include "application/exceptions/app_exceptions.hpp"
#include "application/exceptions/app_base_exception_with_message_impl.hpp"

//------------------------------------------------------------------------------

namespace application {

//------------------------------------------------------------------------------

class CantCreateProjectImpl :
	public BaseExceptionWithMessageImpl< CantCreateProject >
{
	using BaseClass = BaseExceptionWithMessageImpl< CantCreateProject >;

public:

	CantCreateProjectImpl();
};

//------------------------------------------------------------------------------

}
