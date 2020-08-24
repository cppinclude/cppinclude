#include "parser/ih/pr_accessor_impl.hpp"

#include "parser/impl/pr_parser_impl.hpp"

#include <memory>

//------------------------------------------------------------------------------

namespace parser {

//------------------------------------------------------------------------------

ParserAccessorImpl::ParserPtr ParserAccessorImpl::createParser()
{
	return ParserPtr{ new ParserImpl };
}

//------------------------------------------------------------------------------

}
