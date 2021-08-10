#pragma once

#include "parser/ih/pr_accessor.hpp"

//------------------------------------------------------------------------------

namespace parser
{
class Parser;

//------------------------------------------------------------------------------

class ParserAccessorImpl final : public ParserAccessor
{
public:
	ParserPtr createParser() override;
};

//------------------------------------------------------------------------------

}
