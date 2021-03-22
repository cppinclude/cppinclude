#pragma once

#include <string>

//------------------------------------------------------------------------------

namespace parser {

//------------------------------------------------------------------------------

class ParserContext
{
public:
	ParserContext();

	const std::string & getCurrentLine() const noexcept;
	void setCurrentLine( const std::string & _str );

	std::size_t getLineNumber() const noexcept;
	void increaseLineNumber() noexcept;

	bool isEnableMultilineComment() const noexcept;
	void setMultilineComment( bool _enable ) noexcept;

	bool isEnableMultilineString() const noexcept;
	void setMultilineString( bool _enable ) noexcept;

	bool isEnableRawString() const noexcept;
	void seEnableRawString( bool _enable ) noexcept;

private:
	std::string m_currentLine;
	std::size_t m_lineNumber;

	bool m_isEnableMultilineComment;
	bool m_isEnableMultilineString;
	bool m_isEnableRawString;
};

//------------------------------------------------------------------------------

}
