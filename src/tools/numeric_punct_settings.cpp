#include "tools/numeric_punct_settings.hpp"

//------------------------------------------------------------------------------

namespace tools
{
//------------------------------------------------------------------------------

NumericPunctSettings::NumericPunctSettings( char _separator )
	: m_thousandsSeparator( _separator )
{
}

//------------------------------------------------------------------------------

void NumericPunctSettings::setSystemSeparators( char _separator )
{
	std::locale::global( std::locale(
		std::locale( "" ), new tools::NumericPunctSettings{ _separator } ) );
}

//------------------------------------------------------------------------------

char NumericPunctSettings::do_thousands_sep() const
{
	return m_thousandsSeparator;
}

//------------------------------------------------------------------------------

std::string NumericPunctSettings::do_grouping() const
{
	return "\03";
}

//------------------------------------------------------------------------------

}
