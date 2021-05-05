#pragma once

#include <stdfwd/memory>
#include <stdfwd/vector>

//------------------------------------------------------------------------------

namespace fs {
	class File;
}

//------------------------------------------------------------------------------

namespace parser {
	class IncludeFile;

//------------------------------------------------------------------------------

class Parser
{
public:
	virtual ~Parser() = default;

	using IncludeFiles = stdfwd::vector< IncludeFile >;

	virtual IncludeFiles parseFile( const fs::File & _file ) const = 0;
};

//------------------------------------------------------------------------------

}
