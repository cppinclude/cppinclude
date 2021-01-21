#pragma once

#include <stdfwd.hpp>

//------------------------------------------------------------------------------

namespace reporter {

//------------------------------------------------------------------------------

enum class ReporterKind;

//------------------------------------------------------------------------------

ReporterKind toReporterKind( std::string_view _str );
std::string reporterKindToString( ReporterKind _kind );

//------------------------------------------------------------------------------

}
