#pragma once

#include <std_fs>
#include <stdfwd/vector>
#include <stdfwd/string_view>

//------------------------------------------------------------------------------

namespace cmake_project {
	class IncludeParserContext;

//------------------------------------------------------------------------------

class IncludesParser
{
public:

	using Path		= stdfs::path;
	using Includes	= stdfwd::vector< Path >;
	using IndexType	= std::size_t;

	Includes parse( std::string_view _command );

private:

	static IndexType parseArgument(
		IncludeParserContext & _context,
		IndexType startPos
	);

	static IndexType parseInclude(
		IncludeParserContext & _context,
		IndexType startPos
	);

	static bool getIgnoreSpace(
		IncludeParserContext & _context,
		IndexType currentPos
	);

	static IndexType getFinishIgnoreSpace(
		IncludeParserContext & _context,
		IndexType startPos
	);

	static IndexType getSizeSpecialChar();

	static IndexType getNotSpacePos(
		IncludeParserContext & _context,
		IndexType startPos
	);

};

//------------------------------------------------------------------------------

}
