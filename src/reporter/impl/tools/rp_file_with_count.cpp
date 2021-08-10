#include "reporter/impl/tools/rp_file_with_count.hpp"

//------------------------------------------------------------------------------

namespace reporter
{
//------------------------------------------------------------------------------

FileWithCount::FileWithCount(
	const model_includes::File & _file, CountType _count )
	: m_file{ _file }
	, m_count{ _count }
{
}

//------------------------------------------------------------------------------

const model_includes::File & FileWithCount::getFile() const
{
	return m_file;
}

//------------------------------------------------------------------------------

FileWithCount::CountType FileWithCount::getCount() const
{
	return m_count;
}

//------------------------------------------------------------------------------

}
