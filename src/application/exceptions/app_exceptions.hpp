#pragma once

#include "exception/api/exc_exception.hpp"

//------------------------------------------------------------------------------

namespace application
{
//------------------------------------------------------------------------------

class Exception : public exception::Exception
{
};

//------------------------------------------------------------------------------

class CantCreateProject : public Exception
{
};
class CantCreateModel : public Exception
{
};
class CantCreateAnalyzer : public Exception
{
};
class CantCreateReport : public Exception
{
};
class CantLoadReporterSettings : public Exception
{
};
class IncorrectThousandsSeparator : public Exception
{
};

//------------------------------------------------------------------------------

}
