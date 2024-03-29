#include "reporter/impl/most_impact_reporter/rp_most_impact_reporter_detail.hpp"

//------------------------------------------------------------------------------

namespace reporter
{
//------------------------------------------------------------------------------

MostImpactReporterDetail::MostImpactReporterDetail(
	const model_includes::File & _file,
	CountType _includedByCount,
	const model_includes::IncludeLocation & _includeLocation )
	: m_fileInfo{ _file, _includedByCount }
	, m_includeLocation{ _includeLocation }
{
}

//------------------------------------------------------------------------------

const model_includes::File & MostImpactReporterDetail::getFile() const
{
	return getFileInfo().getFile();
}

//------------------------------------------------------------------------------

FileWithCount::CountType MostImpactReporterDetail::getIncludedByCount() const
{
	return getFileInfo().getCount();
}

//------------------------------------------------------------------------------

const FileWithCount & MostImpactReporterDetail::getFileInfo() const
{
	return m_fileInfo;
}

//------------------------------------------------------------------------------

const model_includes::IncludeLocation &
MostImpactReporterDetail::getIncludeLocation() const
{
	return m_includeLocation;
}

//------------------------------------------------------------------------------

}
