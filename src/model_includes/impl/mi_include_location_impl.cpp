#include "model_includes/impl/mi_include_location_impl.hpp"

//------------------------------------------------------------------------------

namespace model_includes
{
//------------------------------------------------------------------------------

IncludeLocationImpl::IncludeLocationImpl(
	LineNumber _line, ColumNumber _begin, ColumNumber _end )
	: m_line{ _line }
	, m_begin{ _begin }
	, m_end{ _end }
{
}

//------------------------------------------------------------------------------

IncludeLocationImpl::LineNumber IncludeLocationImpl::getLineNumber() const
{
	return m_line;
}

//------------------------------------------------------------------------------

IncludeLocationImpl::ColumNumber
IncludeLocationImpl::getFileNameStartPos() const
{
	return m_begin;
}

//------------------------------------------------------------------------------

IncludeLocationImpl::ColumNumber IncludeLocationImpl::getFileNameEndPos() const
{
	return m_end;
}

//------------------------------------------------------------------------------

}
