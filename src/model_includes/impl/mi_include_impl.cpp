#include "model_includes/impl/mi_include_impl.hpp"

#include "model_includes/api/mi_include_location.hpp"

#include "exception/ih/exc_internal_error.hpp"

//------------------------------------------------------------------------------

namespace model_includes {

//------------------------------------------------------------------------------

IncludeImpl::IncludeImpl(
	IncludeLocationPtr && _location,
	const File & _sourceFile,
	const File & _destinationFile,
	IncludeStatus _status,
	IncludeType _type
)
	:	m_location{ std::move(_location ) }
	,	m_sourceFile{ _sourceFile }
	,	m_destinationFile{ _destinationFile }
	,	m_status{ _status }
	,	m_type{ _type }
{
}

//------------------------------------------------------------------------------

IncludeImpl::~IncludeImpl() = default;

//------------------------------------------------------------------------------

const IncludeLocation & IncludeImpl::getLocation() const
{
	INTERNAL_CHECK_ERROR( m_location );
	return *m_location;
}

//------------------------------------------------------------------------------

const File & IncludeImpl::getSourceFile() const
{
	return m_sourceFile;
}

//------------------------------------------------------------------------------

const File & IncludeImpl::getDestinationFile() const
{
	return m_destinationFile;
}

//------------------------------------------------------------------------------

IncludeStatus IncludeImpl::getStatus() const
{
	return m_status;
}

//------------------------------------------------------------------------------

IncludeType IncludeImpl::getType() const
{
	return m_type;
}

//------------------------------------------------------------------------------

}
