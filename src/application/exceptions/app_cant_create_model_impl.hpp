#pragma once

#include "application/exceptions/app_base_exception_with_message_impl.hpp"
#include "application/exceptions/app_exceptions.hpp"

//------------------------------------------------------------------------------

namespace application
{
//------------------------------------------------------------------------------

class CantCreateModelImpl :
	public BaseExceptionWithMessageImpl< CantCreateModel >
{
	using BaseClass = BaseExceptionWithMessageImpl< CantCreateModel >;

public:
	CantCreateModelImpl();
};

//------------------------------------------------------------------------------

}
