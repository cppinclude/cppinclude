#include "reporter/impl/impact_reporter/rp_file_info.hpp"

//------------------------------------------------------------------------------

namespace reporter {

//------------------------------------------------------------------------------

FileInfo::FileInfo(
	const model_includes::File & _file,
	CountType _includedByCount
)
	:	m_file{ _file }
	,	m_includedByCount{ _includedByCount }
{
}

//------------------------------------------------------------------------------

const model_includes::File & FileInfo::getFile() const
{
	return m_file;
}

//------------------------------------------------------------------------------

FileInfo::CountType FileInfo::getIncludedByCount() const
{
	return m_includedByCount;
}

//------------------------------------------------------------------------------

}
