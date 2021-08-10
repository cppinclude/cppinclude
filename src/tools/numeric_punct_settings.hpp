#pragma once

#include <locale>

//------------------------------------------------------------------------------

namespace tools
{
//------------------------------------------------------------------------------

class NumericPunctSettings final : public std::numpunct< char >
{
public:
	explicit NumericPunctSettings( char _separator );

	static void setSystemSeparators( char _separator );

protected:
	char do_thousands_sep() const override;
	std::string do_grouping() const override;

private:
	const char m_thousandsSeparator;
};

//------------------------------------------------------------------------------

}
