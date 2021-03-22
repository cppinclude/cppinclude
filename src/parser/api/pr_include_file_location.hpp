#pragma once

#include <stdfwd.hpp>

//------------------------------------------------------------------------------

namespace parser {

//------------------------------------------------------------------------------

class IncludeFileLocation
{
public:

	IncludeFileLocation( std::size_t _line, std::size_t _begin, std::size_t _end )
		:	m_line{ _line }
		,	m_begin{ _begin }
		,	m_end{ _end }
	{
	}

	std::size_t getLineNumber() const
	{
		return m_line;
	}
	std::size_t getBegin() const
	{
		return m_begin;
	}
	std::size_t getEnd() const
	{
		return m_end;
	}

private:
	const std::size_t m_line;
	const std::size_t m_begin;
	const std::size_t m_end;
};

//------------------------------------------------------------------------------

}
