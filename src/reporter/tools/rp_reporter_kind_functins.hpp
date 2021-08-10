#pragma once

#include <stdfwd/string>
#include <stdfwd/string_view>

//------------------------------------------------------------------------------

namespace reporter
{
//------------------------------------------------------------------------------

enum class ReporterKind;

//------------------------------------------------------------------------------

ReporterKind toReporterKind( std::string_view _str );
std::string reporterKindToString( ReporterKind _kind );

//------------------------------------------------------------------------------

}
