#pragma once

#include <cstddef>

//------------------------------------------------------------------------------

namespace model_includes {
	class File;
}

//------------------------------------------------------------------------------

namespace reporter {

//------------------------------------------------------------------------------

class FileWithCount
{

public:

	using CountType = std::size_t;

	FileWithCount( const model_includes::File & _file,  CountType _count );

	const model_includes::File & getFile() const;
	CountType getCount() const;

private:

	const model_includes::File & m_file;
	const CountType m_count;
};

//------------------------------------------------------------------------------

}
