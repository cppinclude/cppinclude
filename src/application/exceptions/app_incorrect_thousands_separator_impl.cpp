#include "application/exceptions/app_incorrect_thousands_separator_impl.hpp"

#include "application/resources/app_resources_exceptions.hpp"

//------------------------------------------------------------------------------

namespace application
{
//------------------------------------------------------------------------------

IncorrectThousandsSeparatorImpl::IncorrectThousandsSeparatorImpl(
	const std::string & _separator )
	: BaseClass(
		  resources::exceptions::IncorrectThousandsSeparator::Code,
		  resources::exceptions::IncorrectThousandsSeparator::Msg + _separator )
{
}

//------------------------------------------------------------------------------

}
