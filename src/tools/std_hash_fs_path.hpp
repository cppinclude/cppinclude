#pragma once

#include <filesystem>

//------------------------------------------------------------------------------

namespace std
{
	template<> struct hash<filesystem::path>
	{
		std::size_t operator()(const filesystem::path & _file) const noexcept
		{
			return std::filesystem::hash_value( _file );
		}
	};
}

//------------------------------------------------------------------------------
