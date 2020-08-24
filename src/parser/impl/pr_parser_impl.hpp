#pragma once

#include "parser/api/pr_parser.hpp"

//------------------------------------------------------------------------------

namespace parser {

//------------------------------------------------------------------------------

class ParserImpl final : public Parser
{

public:

	IncludeFiles parseFile( const fs::File & _file ) const override;

private:

	using IncludeFileOpt = std::optional< IncludeFile >;

	IncludeFileOpt parseLine(
		std::string_view _line,
		std::size_t _lineNumber
	) const;

	static std::size_t findComentEnd(
		std::string_view _line,
		std::size_t _index
	) noexcept;
	static std::size_t findEndOfString(
		std::string_view _line,
		std::size_t _index
	) noexcept;
	static std::optional< std::size_t > findInclude(
		std::string_view _line,
		std::size_t _index
	);
	static IncludeFileOpt parseInclude(
		std::string_view _line,
		std::size_t _lineNumber,
		std::size_t _index
	);
};

//------------------------------------------------------------------------------

}
