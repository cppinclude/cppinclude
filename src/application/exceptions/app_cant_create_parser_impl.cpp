#include "application/exceptions/app_cant_create_parser_impl.hpp"

#include "application/resources/app_resources_exceptions.hpp"

//------------------------------------------------------------------------------

namespace application {

//------------------------------------------------------------------------------

CantCreateParserImpl::CantCreateParserImpl()
	:	BaseClass(
			resources::exceptions::CantCreateParser::Code,
			resources::exceptions::CantCreateParser::Msg
	)
{

}

//------------------------------------------------------------------------------

}
