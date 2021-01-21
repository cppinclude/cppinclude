#pragma once

#include "reporter/impl/tools/rp_file_with_count.hpp"

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

	using CountType = FileWithCount::CountType;

	MostImpactReporterDetail(
		const model_includes::File & _file,
		CountType _includedByCount,
		const model_includes::IncludeLocation & _includeLocation
	);

public:

	const model_includes::File & getFile() const;
	FileWithCount::CountType getIncludedByCount() const;
	const model_includes::IncludeLocation & getIncludeLocation() const;

	const FileWithCount & getFileInfo() const;

private:

	const FileWithCount m_fileInfo;
	const model_includes::IncludeLocation & m_includeLocation;
};

//------------------------------------------------------------------------------

}
