#pragma once

#include <stdfwd/string_view>
#include <stdfwd/string>

//------------------------------------------------------------------------------

namespace reporter {

//------------------------------------------------------------------------------

enum class ReporterKind;

//------------------------------------------------------------------------------

ReporterKind toReporterKind( std::string_view _str );
std::string reporterKindToString( ReporterKind _kind );

//------------------------------------------------------------------------------

}
