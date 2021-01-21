#pragma once

#include "reporter/exceptions/rp_exceptions.hpp"
#include "reporter/exceptions/rp_base_exception_impl.hpp"

//------------------------------------------------------------------------------

namespace reporter {

//------------------------------------------------------------------------------

class IncorrectReportImpl : public BaseExceptionImpl< IncorrectReport >
{
	using BaseClass = BaseExceptionImpl< IncorrectReport >;

public:

	explicit IncorrectReportImpl( std::string_view _name );

	std::string getMessage() const noexcept override;

private:

	static std::string getPossiblesValues();

private:

	const std::string m_name;

};

//------------------------------------------------------------------------------

}
