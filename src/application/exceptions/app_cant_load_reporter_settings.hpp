#pragma once

#include "application/exceptions/app_base_exception_with_message_impl.hpp"
#include "application/exceptions/app_exceptions.hpp"

//------------------------------------------------------------------------------

namespace application
{
//------------------------------------------------------------------------------

class CantLoadReporterSettingsImpl
	: public BaseExceptionWithMessageImpl< CantLoadReporterSettings >
{
	using BaseClass = BaseExceptionWithMessageImpl< CantLoadReporterSettings >;

public:
	CantLoadReporterSettingsImpl();
};

//------------------------------------------------------------------------------

}
