#include "application/exceptions/app_cant_create_analyzer.hpp"

#include "application/resources/app_resources_exceptions.hpp"

//------------------------------------------------------------------------------

namespace application
{
//------------------------------------------------------------------------------

CantCreateAnalyzerImpl::CantCreateAnalyzerImpl()
	:	BaseClass(
			resources::exceptions::CantCreateAnalyzer::Code,
			resources::exceptions::CantCreateAnalyzer::Msg
	)
{
}

//------------------------------------------------------------------------------

}
