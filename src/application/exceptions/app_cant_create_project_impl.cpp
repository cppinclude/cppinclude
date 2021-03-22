#include "application/exceptions/app_cant_create_project_impl.hpp"

#include "application/resources/app_resources_exceptions.hpp"

//------------------------------------------------------------------------------

namespace application
{
//------------------------------------------------------------------------------

CantCreateProjectImpl::CantCreateProjectImpl()
	:	BaseClass(
			resources::exceptions::CantCreateProject::Code,
			resources::exceptions::CantCreateProject::Msg
	)
{
}

//------------------------------------------------------------------------------

}
