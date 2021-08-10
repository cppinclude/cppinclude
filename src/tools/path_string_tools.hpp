#pragma once

#include <string>

//------------------------------------------------------------------------------

namespace tools
{
//------------------------------------------------------------------------------

std::string toPath( const std::string & _originStr );

std::string toUnixPath( const std::string & _originStr );
std::string toWindowsPath( const std::string & _originStr );

std::string changeSeparatorInPath(
	const std::string & _originStr, char _oldSeparator, char _newSeparator );

//------------------------------------------------------------------------------

}
