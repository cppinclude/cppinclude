#pragma once

#include "parser/api/pr_parser.hpp"

//------------------------------------------------------------------------------

namespace parser {

class ParserContext;

//------------------------------------------------------------------------------

class ParserImpl final : public Parser
{

public:

	IncludeFiles parseFile( const fs::File & _file ) const override;

private:

	using IncludeFileOpt = std::optional< IncludeFile >;

	static IncludeFileOpt parseLine( ParserContext & _context );
	static IncludeFileOpt parseInclude(
		const ParserContext & _context,
		std::size_t _index
	);

	static std::size_t getStartPos( ParserContext & _context ) noexcept;

	static std::size_t findComentEnd(
		ParserContext & _context,
		std::size_t _index
	) noexcept;
	static std::size_t findEndOfString(
		ParserContext & _context,
		std::size_t _index
	) noexcept;
	static std::optional< std::size_t > findInclude(
		std::string_view _line,
		std::size_t _index
	);


};

//------------------------------------------------------------------------------

}
