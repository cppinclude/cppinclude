#include "application/exceptions/app_cant_load_reporter_settings.hpp"

#include "application/resources/app_resources_exceptions.hpp"

//------------------------------------------------------------------------------

namespace application
{
//------------------------------------------------------------------------------

CantLoadReporterSettingsImpl::CantLoadReporterSettingsImpl()
	: BaseClass(
		  resources::exceptions::CantLoadReporterSettings::Code,
		  resources::exceptions::CantLoadReporterSettings::Msg )
{
}

//------------------------------------------------------------------------------

}
