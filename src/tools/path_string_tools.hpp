#pragma once

#include <string>

//------------------------------------------------------------------------------

namespace tools {

//------------------------------------------------------------------------------

std::string toPath( const std::string & _originStr );

std::string toUnixPath( const std::string & _originStr );
std::string toWindowsPath( const std::string & _originStr );

std::string changeSeperatorInPath(
    const std::string & _originStr,
    char _oldSeperator,
    char _newSeperator
);

//------------------------------------------------------------------------------

}
