#pragma once

#include "model_includes/api/mi_model.hpp"

#include "model_includes/api/mi_include.hpp"

#include "tools/std_hash_fs_path.hpp"

#include <unordered_map>
#include <vector>

//------------------------------------------------------------------------------

namespace model_includes {

//------------------------------------------------------------------------------

class ModelImpl final : public Model
{
public:

	ModelImpl();
	~ModelImpl() override;

	std::size_t getFilesCount() const override;
	void forEachFile( FileCallback _callback ) const override;

	void setProjectDir( const Path & _path ) override;
	const Path & getProjectDir() const override;

	File & ensureFile( const Path & _filePath, FileType _fileType ) override;
	const File * findFile( const Path & _filePath ) const override;

	void forEachInclude( IncludeCallback _callback ) const override;

	const Include & createInclude(
		const IncludeLocationInfo & _location,
		File & _sourceFile,
		File & _destinationFile,
		IncludeStatus _status,
		IncludeType _type
	) override;

private:

	using IncludeLocationPtr = std::unique_ptr< IncludeLocation >;
	IncludeLocationPtr createIncludeLocation(
		const IncludeLocationInfo & _location
	) const;

private:

	using FilePtr = std::unique_ptr< File >;
	using Files = std::unordered_map< Path, FilePtr >;

	using IncludePtr = std::unique_ptr< Include >;
	using Includes = std::vector< IncludePtr >;

	Files m_files;
	Includes m_includes;

	Path m_projectDir;

};

//------------------------------------------------------------------------------

}
