#pragma once

#include "application/exceptions/app_exceptions.hpp"
#include "application/exceptions/app_base_exception_with_message_impl.hpp"

//------------------------------------------------------------------------------

namespace application {

//------------------------------------------------------------------------------

class CantLoadReporterSettingsImpl :
	public BaseExceptionWithMessageImpl< CantLoadReporterSettings >
{
	using BaseClass = BaseExceptionWithMessageImpl< CantLoadReporterSettings >;

public:

	CantLoadReporterSettingsImpl();
};

//------------------------------------------------------------------------------

}
