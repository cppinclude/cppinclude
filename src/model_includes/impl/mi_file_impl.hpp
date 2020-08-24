#pragma once

#include "model_includes/api/mi_file.hpp"
#include "model_includes/api/enums/mi_file_type.hpp"

#include <filesystem>
#include <vector>

//------------------------------------------------------------------------------

namespace model_includes {

//------------------------------------------------------------------------------

class FileImpl final : public File
{
public:

	FileImpl( const std::filesystem::path & _path, FileType _type );

	std::filesystem::path getPath() const override;
	FileType getType() const override;

	void addInclude( const Include & _include ) override;

	IncludeIndex getIncludesCount() const override;
	IncludeIndex getIncludeFilesCountRecursive() const override;
	const Include & getInclude( IncludeIndex _index ) const override;

	IncludeIndex getIncludedByCount() const override;
	IncludeIndex getIncludedByFilesCountRecursive() const override;
	const Include & getIncludedBy( IncludeIndex _index ) const override;

private:

	using IncludesContainer = std::vector< const Include * >;
	using IncludeCallback = std::function< void ( const Include & ) >;

	IncludeIndex getCountRecursive(
		IncludeIndex ( File::*_getCount )() const,
		const Include & ( File::*_getInclude )( IncludeIndex _index ) const,
		const File & ( Include::*_getFile )() const
	) const;

private:

	IncludesContainer m_includes;
	IncludesContainer m_includedBy;

	const std::filesystem::path m_path;
	const FileType m_type;
};

//------------------------------------------------------------------------------

}
