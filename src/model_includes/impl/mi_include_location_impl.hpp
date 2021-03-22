#pragma once

#include "model_includes/api/mi_include_location.hpp"

//------------------------------------------------------------------------------

namespace model_includes {

//------------------------------------------------------------------------------

class IncludeLocationImpl : public IncludeLocation
{
public:
	IncludeLocationImpl(
		LineNumber _line,
		ColumNumber _begin,
		ColumNumber _end
	);

	LineNumber getLineNumber() const override;
	ColumNumber getFileNameStartPos() const override;
	ColumNumber getFileNameEndPos() const override;

private:
	LineNumber m_line;
	ColumNumber m_begin;
	ColumNumber m_end;
};

//------------------------------------------------------------------------------

}
