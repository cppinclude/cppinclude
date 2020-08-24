#pragma once

#include <stdfwd.hpp>

//------------------------------------------------------------------------------

namespace model_includes {

//------------------------------------------------------------------------------

class IncludeLocation
{
public:

	virtual ~IncludeLocation() = default;

	using LineNumber	= std::size_t;
	using ColumNumber	= std::size_t;

	virtual LineNumber getLineNumber() const = 0;
	virtual ColumNumber getFileNameStartPos() const = 0;
	virtual ColumNumber getFileNameEndPos() const = 0;
};

//------------------------------------------------------------------------------

}
