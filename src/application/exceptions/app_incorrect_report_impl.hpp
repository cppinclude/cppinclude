#pragma once

#include "application/exceptions/app_exceptions.hpp"
#include "application/exceptions/app_base_exception_impl.hpp"

#include "reporter/api/enums/rp_reporter_kind.hpp"

#include <unordered_map>

//------------------------------------------------------------------------------

namespace application {

//------------------------------------------------------------------------------

class IncorrectReportImpl : public BaseExceptionImpl< IncorrectReport >
{
	using BaseClass = BaseExceptionImpl< IncorrectReport >;

public:

	using Names = std::unordered_map< std::string, reporter::ReporterKind >;

	explicit IncorrectReportImpl( std::string_view _name, const Names & _names );

	std::string getMessage() const noexcept override;

private:

	std::string getNames() const;

private:

	const Names m_possibleNames;

	const std::string m_name;

};

//------------------------------------------------------------------------------

}
