#include "tools/path_string_tools.hpp"

#include <algorithm>

//------------------------------------------------------------------------------

namespace tools
{
//------------------------------------------------------------------------------

std::string toPath( const std::string & _originStr )
{
#ifdef _WIN32
	return toWindowsPath( _originStr );
#else
	return toUnixPath( _originStr );
#endif
}

//------------------------------------------------------------------------------

std::string toUnixPath( const std::string & _originStr )
{
	return changeSeparatorInPath( _originStr, '\\', '/' );
}

//------------------------------------------------------------------------------

std::string toWindowsPath( const std::string & _originStr )
{
	return changeSeparatorInPath( _originStr, '/', '\\' );
}

//------------------------------------------------------------------------------

std::string changeSeparatorInPath(
	const std::string & _originStr, char _oldSeparator, char _newSeparator )
{
	std::string result{ _originStr };

	std::replace_if(
		result.begin(), result.end(),
		[&]( char _c ) { return _c == _oldSeparator; }, _newSeparator );

	return result;
}

//------------------------------------------------------------------------------

}
