#pragma once

#include "reporter/impl/impact_reporter/rp_file_info.hpp"

//------------------------------------------------------------------------------

namespace model_includes {
	class IncludeLocation;
	class File;
}

//------------------------------------------------------------------------------

namespace reporter {

//------------------------------------------------------------------------------

class MostImpactReporterDetail
{
public:

	using CountType = FileInfo::CountType;

	MostImpactReporterDetail(
		const model_includes::File & _file,
		CountType _includedByCount,
		const model_includes::IncludeLocation & _includeLocation
	);

public:

	const model_includes::File & getFile() const;
	FileInfo::CountType getIncludedByCount() const;
	const model_includes::IncludeLocation & getIncludeLocation() const;

	const FileInfo & getFileInfo() const;

private:

	const FileInfo m_fileInfo;
	const model_includes::IncludeLocation & m_includeLocation;
};

//------------------------------------------------------------------------------

}
