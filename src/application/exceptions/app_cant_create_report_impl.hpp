#pragma once

#include "application/exceptions/app_exceptions.hpp"
#include "application/exceptions/app_base_exception_impl.hpp"

#include "reporter/api/enums/rp_reporter_kind.hpp"

//------------------------------------------------------------------------------

namespace application {

//------------------------------------------------------------------------------

class CantCreateReportImpl : public BaseExceptionImpl< CantCreateReport >
{
	using BaseClass = BaseExceptionImpl< CantCreateReport >;

public:

	CantCreateReportImpl( reporter::ReporterKind _kind );

	std::string getMessage() const noexcept override;

private:

	static std::string toString( reporter::ReporterKind _kind );

private:

	const reporter::ReporterKind m_kind;
};

//------------------------------------------------------------------------------

}
