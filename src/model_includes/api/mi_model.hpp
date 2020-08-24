#pragma once

#include <stdfwd.hpp>

//------------------------------------------------------------------------------

namespace model_includes {
	class File;
	class Include;
	enum class FileType;
	enum class IncludeStatus;
	enum class IncludeType;

//------------------------------------------------------------------------------

class Model
{
public:

	using FileCallback		= std::function< bool ( const File & ) >;
	using IncludeCallback	= std::function< bool ( const Include & ) >;

	virtual ~Model() = default;

	virtual std::size_t getFilesCount() const = 0;
	virtual void forEachFile( FileCallback _callback ) const = 0;

	virtual void setProjectDir( const std::filesystem::path & _path ) = 0;
	virtual const std::filesystem::path & getProjectDir() const = 0;

	virtual const File * findFile(
		const std::filesystem::path & _filePath
	) const = 0;
	virtual File & ensureFile(
		const std::filesystem::path & _filePath,
		FileType _fileType
	) = 0;

	virtual void forEachInclude( IncludeCallback _callback ) const = 0;

	struct IncludeLocationInfo
	{
		const std::size_t m_line;
		const std::size_t m_begin;
		const std::size_t m_end;
	};

	virtual const Include & createInclude(
		const IncludeLocationInfo & _location,
		File & _sourceFile,
		File & _destinationFile,
		IncludeStatus _status,
		IncludeType _type
	) = 0;
};

//------------------------------------------------------------------------------

}

//------------------------------------------------------------------------------
