#pragma once

#include "project/api/prj_exceptions.hpp"
#include "project/impl/exceptions/prj_base_exception_impl.hpp"

//------------------------------------------------------------------------------

namespace project
{
//------------------------------------------------------------------------------

class InvalidRegexImpl : public BaseExceptionImpl< InvalidRegex >
{
	using BaseClass = BaseExceptionImpl< InvalidRegex >;

public:
	InvalidRegexImpl( const std::exception & _originException );

	std::string getMessage() const noexcept override;

private:
	const std::string m_originMsg;
};

//------------------------------------------------------------------------------

}
