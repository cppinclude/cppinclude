#include "parser/impl/pr_parser_context.hpp"

//------------------------------------------------------------------------------

namespace parser {

//------------------------------------------------------------------------------

ParserContext::ParserContext()
	:	m_lineNumber{1}
	,	m_isEnableMultilineComment{ false }
	,	m_isEnableMultilineString{ false }
	,	m_isEnableRawString{ false }
{

}

//------------------------------------------------------------------------------

const std::string & ParserContext::getCurrentLine() const noexcept
{
	return m_currentLine;
}

//------------------------------------------------------------------------------

void ParserContext::setCurrentLine( const std::string & _str )
{
	m_currentLine = _str;
}

//------------------------------------------------------------------------------

std::size_t ParserContext::getLineNumber() const noexcept
{
	return m_lineNumber;
}

//------------------------------------------------------------------------------

void ParserContext::increaseLineNumber() noexcept
{
	++m_lineNumber;
}

//------------------------------------------------------------------------------

bool ParserContext::isEnableMultilineComment() const noexcept
{
	return m_isEnableMultilineComment;
}

//------------------------------------------------------------------------------

void ParserContext::setMultilineComment( bool _enable ) noexcept
{
	m_isEnableMultilineComment = _enable;
}

//------------------------------------------------------------------------------

bool ParserContext::isEnableMultilineString() const noexcept
{
	return m_isEnableMultilineString;
}

//------------------------------------------------------------------------------

void ParserContext::setMultilineString( bool _enable ) noexcept
{
	m_isEnableMultilineString = _enable;
}

//------------------------------------------------------------------------------

bool ParserContext::isEnableRawString() const noexcept
{
	return m_isEnableRawString;
}

//------------------------------------------------------------------------------

void ParserContext::seEnableRawString( bool _enable ) noexcept
{
	m_isEnableRawString = _enable;
}

//------------------------------------------------------------------------------

}
