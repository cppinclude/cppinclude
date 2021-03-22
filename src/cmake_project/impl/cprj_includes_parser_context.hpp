#pragma once

#include <std_fs>
#include <stdfwd.hpp>

#include <string>
#include <vector>

//------------------------------------------------------------------------------

namespace cmake_project {

//------------------------------------------------------------------------------

class IncludeParserContext
{
public:

	using Path		= stdfs::path;
	using Includes	= stdfwd::vector< Path >;
	using IndexType	= std::size_t;

	IncludeParserContext( std::string_view _command );

	const std::string & getCommand() const;
	IndexType getCommandSize() const;

	const Includes & getIncludes() const;

	void addInclude( const Path & _path );

private:
	IndexType m_size;
	std::string m_command;

	Includes m_includes;
};

//------------------------------------------------------------------------------

}
