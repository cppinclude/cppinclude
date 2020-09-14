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

private:

	std::string m_currentLine;
	std::size_t m_lineNumber = 1;

	bool m_isEnableMultilineComment = false;
	bool m_isEnableMultilineString = false;

};

//------------------------------------------------------------------------------

}
