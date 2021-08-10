#include "reporter/resources/rp_resources_exceptions.hpp"

//------------------------------------------------------------------------------

namespace reporter::resources::exceptions
{
//------------------------------------------------------------------------------

const char * const ModuleName = "REPORT";

//------------------------------------------------------------------------------

namespace IncorrectReport
{
const char * const Code = "INCORRECT_REPORT";
const char * const MsgFmt = "Incorrect report name: {}, possible names: {}";
}

//------------------------------------------------------------------------------

}
