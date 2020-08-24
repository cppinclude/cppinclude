#pragma once

#include "exception/ih/exc_exception_impl.hpp"

//------------------------------------------------------------------------------

namespace exception {

//------------------------------------------------------------------------------

template< class _BaseException >
class ExceptionWithMessageImpl : public ExceptionImpl< _BaseException >
{
	using BaseClass = ExceptionImpl< _BaseException >;

public:

	ExceptionWithMessageImpl(
		std::string_view _module,
		std::string_view _code,
		std::string_view _message
	)
		:	BaseClass{ _module, _code }
		,	m_message{ _message }
	{

	}

	std::string getMessage() const noexcept override
	{
		return m_message;
	}

private:

	const std::string m_message;
};

//------------------------------------------------------------------------------

}
