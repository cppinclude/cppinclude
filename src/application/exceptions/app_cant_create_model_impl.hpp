#pragma once

#include "application/exceptions/app_exceptions.hpp"
#include "application/exceptions/app_base_exception_with_message_impl.hpp"

//------------------------------------------------------------------------------

namespace application {

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
