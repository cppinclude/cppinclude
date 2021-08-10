#include "application/tools/app_log.hpp"

#include <iostream>
#include <string_view>

//------------------------------------------------------------------------------

namespace application
{
//------------------------------------------------------------------------------

Log::Log()
	: m_stream{ std::cout }
	, m_errorStream{ std::cerr }
{
}

//------------------------------------------------------------------------------

Log::Log( std::ostream & _stream, std::ostream & _errorStream )
	: m_stream{ _stream }
	, m_errorStream{ _errorStream }
{
}

//------------------------------------------------------------------------------

void Log::printLine( std::string_view _line )
{
	m_stream << _line << std::endl;
}

//------------------------------------------------------------------------------

void Log::printWarning( const std::exception & _exception )
{
	m_errorStream << _exception.what() << std::endl;
}

//------------------------------------------------------------------------------

}
