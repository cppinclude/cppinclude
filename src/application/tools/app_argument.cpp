#include "application/tools/app_argument.hpp"

//------------------------------------------------------------------------------

namespace application
{
//------------------------------------------------------------------------------

Argument::Argument(
	std::string _fullName, std::string _description, ValueOpt _defaultValue )
	: m_fullName{ std::move( _fullName ) }
	, m_description{ std::move( _description ) }
	, m_defaultValue{ std::move( _defaultValue ) }
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
