#include "cmake_project/impl/cprj_includes_parser_context.hpp"

#include <string_view>

//------------------------------------------------------------------------------

namespace cmake_project {

//------------------------------------------------------------------------------

IncludeParserContext::IncludeParserContext( std::string_view _command )
	:	m_command{ _command }
	,	m_size{ _command.size() }
{
}

//------------------------------------------------------------------------------

const std::string & IncludeParserContext::getCommand() const
{
	return m_command;
}

//------------------------------------------------------------------------------

IncludeParserContext::IndexType IncludeParserContext::getCommandSize() const
{
	return m_size;
}

//------------------------------------------------------------------------------

const IncludeParserContext::Includes & IncludeParserContext::getIncludes() const
{
	return m_includes;
}

//------------------------------------------------------------------------------

void IncludeParserContext::addInclude( const Path & _path )
{
	m_includes.push_back( _path );
}

//------------------------------------------------------------------------------

}
