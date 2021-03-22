#pragma once

#include "application/exceptions/app_base_exception_with_message_impl.hpp"
#include "application/exceptions/app_exceptions.hpp"

//------------------------------------------------------------------------------

namespace application
{
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
