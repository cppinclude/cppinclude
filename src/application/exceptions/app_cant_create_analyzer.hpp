#pragma once

#include "application/exceptions/app_base_exception_with_message_impl.hpp"
#include "application/exceptions/app_exceptions.hpp"

//------------------------------------------------------------------------------

namespace application
{
//------------------------------------------------------------------------------

class CantCreateAnalyzerImpl
	: public BaseExceptionWithMessageImpl< CantCreateAnalyzer >
{
	using BaseClass = BaseExceptionWithMessageImpl< CantCreateAnalyzer >;

public:
	CantCreateAnalyzerImpl();
};

//------------------------------------------------------------------------------

}
