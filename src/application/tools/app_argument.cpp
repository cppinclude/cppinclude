#include "application/tools/app_argument.hpp"

//------------------------------------------------------------------------------

namespace application {

//------------------------------------------------------------------------------

Argument::Argument(
	const std::string & _fullName,
	const std::string & _description,
	ValueOpt _defaultValue
)
	:	m_fullName{ _fullName }
	,	m_description{ _description }
	,	m_defaultValue{ _defaultValue }
{
}

//------------------------------------------------------------------------------

const std::string & Argument::getFullName() const
{
	return m_fullName;
}

//------------------------------------------------------------------------------

const std::string & Argument::getDescription() const
{
	return m_description;
}

//------------------------------------------------------------------------------

}
