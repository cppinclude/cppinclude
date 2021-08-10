#pragma once

#include "exception/ih/exc_exception_impl.hpp"

#include <iostream>
#include <string>

//------------------------------------------------------------------------------

namespace exception
{
//------------------------------------------------------------------------------

class InternalError : public ExceptionImpl< Exception >
{
	using BaseClass = ExceptionImpl< Exception >;

public:
	InternalError(
		std::string_view _file, std::string_view _function, int _line )
		: BaseClass( "EXC", "INTERNAL_ERROR" )
		, m_file{ _file }
		, m_function{ _function }
		, m_line{ _line }
	{
	}

	std::string getMessage() const noexcept override
	{
		return "Internal error " + m_file + ":" + std::to_string( m_line ) +
			   " " + m_function;
	}

private:
	const std::string m_file;
	const std::string m_function;
	const int m_line;
};

//------------------------------------------------------------------------------

}

//------------------------------------------------------------------------------

#define INTERNAL_ERROR \
	::exception::InternalError( __FILE__, __FUNCTION__, __LINE__ )

//------------------------------------------------------------------------------

#define THROW_INTERNAL_ERROR throw INTERNAL_ERROR;

//------------------------------------------------------------------------------

#define INTERNAL_CHECK_ERROR( _condition ) \
	if( !( _condition ) )                  \
	THROW_INTERNAL_ERROR

//------------------------------------------------------------------------------

#define INTERNAL_CHECK_WARRING( _condition ) \
	if( !( _condition ) )                    \
		std::cout << INTERNAL_ERROR.what() << std::endl;

//------------------------------------------------------------------------------
