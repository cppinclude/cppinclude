#pragma once

#include <stdfwd.hpp>

//------------------------------------------------------------------------------

namespace model_includes {
	class Include;
	enum class FileType;

//------------------------------------------------------------------------------

class File
{
public:

	using IncludeIndex = std::size_t;

	virtual ~File() = default;

	virtual std::filesystem::path getPath() const = 0;
	virtual FileType getType() const = 0;

	virtual void addInclude( const Include & _include ) = 0;

	virtual IncludeIndex getIncludesCount() const = 0;
	virtual IncludeIndex getIncludeFilesCountRecursive() const = 0;
	virtual const Include & getInclude( IncludeIndex _index ) const = 0;

	virtual IncludeIndex getIncludedByCount() const = 0;
	virtual IncludeIndex getIncludedByFilesCountRecursive() const = 0;
	virtual const Include & getIncludedBy( IncludeIndex _index ) const = 0;
};

//------------------------------------------------------------------------------

}
