#pragma once

#include "exception/api/exc_exception.hpp"

//------------------------------------------------------------------------------

namespace fs {

//------------------------------------------------------------------------------

class Exception : public exception::Exception {};

//------------------------------------------------------------------------------

class CantOpenFile : public Exception {};
class CantCreateFile : public Exception {};

//------------------------------------------------------------------------------

}
