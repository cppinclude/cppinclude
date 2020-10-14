#pragma once

#include <std_fs>

//------------------------------------------------------------------------------

namespace std
{
	template<> struct hash<stdfs::path>
	{
		std::size_t operator()(const stdfs::path & _file) const noexcept
		{
			return stdfs::hash_value( _file );
		}
	};
}

//------------------------------------------------------------------------------
