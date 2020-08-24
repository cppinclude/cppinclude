#pragma once

#include <cstddef>

//------------------------------------------------------------------------------

namespace model_includes {
	class File;
}

//------------------------------------------------------------------------------

namespace reporter {

//------------------------------------------------------------------------------

class FileInfo
{

public:

	using CountType = std::size_t;

	FileInfo(
		const model_includes::File & _file,
		CountType _includedByCount
	);

public:

	const model_includes::File & getFile() const;

	CountType getIncludedByCount() const;

private:

	const model_includes::File & m_file;
	const CountType m_includedByCount;
};

//------------------------------------------------------------------------------

}
