#include "application/exceptions/app_cant_create_model_impl.hpp"
#include "application/resources/app_resources_exceptions.hpp"

//------------------------------------------------------------------------------

namespace application {

//------------------------------------------------------------------------------

CantCreateModelImpl::CantCreateModelImpl()
	:	BaseClass(
			resources::exceptions::CantCreateModel::Code,
			resources::exceptions::CantCreateModel::Msg
	)
{

}

//------------------------------------------------------------------------------

}
