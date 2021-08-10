#pragma once

#include <std_fs>
#include <vector>

#include <string>
#include <string_view>

//------------------------------------------------------------------------------

namespace cmake_project
{
//------------------------------------------------------------------------------

class IncludeParserContext
{
public:
	using Path = stdfs::path;
	using Includes = std::vector< Path >;
	using IndexType = std::size_t;

	IncludeParserContext( std::string_view _command );

	const std::string & getCommand() const;
	IndexType getCommandSize() const;

	const Includes & getIncludes() const;

	void addInclude( const Path & _path );

private:
	std::vector< Path > m_includes;

	std::string m_command;
	IndexType m_size;
};

//------------------------------------------------------------------------------

}
