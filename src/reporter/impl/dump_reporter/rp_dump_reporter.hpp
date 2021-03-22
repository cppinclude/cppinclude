#pragma once

#include "reporter/impl/rp_base_reporter_impl.hpp"
#include "reporter/impl/tools/rp_sorted_files_by_name_container.hpp"

//------------------------------------------------------------------------------

namespace model_includes {
	class Include;
	class File;

	enum class FileType;
	enum class IncludeType;
	enum class IncludeStatus;
}

//------------------------------------------------------------------------------

namespace reporter {

//------------------------------------------------------------------------------

class DumpReporter final : public BaseReporterImpl
{
	using BaseClass = BaseReporterImpl;

public:
	explicit DumpReporter( SettingsPtr && _settingsPtr );

	void report(
		const model_includes::Model & _model,
		std::ostream & _stream
	) override;

	ReporterKind getKind() const override;

private:
	using Path = stdfs::path;

	using Files = SortedFilesByNameContainer;

	void collectFiles( const model_includes::Model & _model, Files & _files ) const;

	void dump(
		const Files & _files,
		const Path & _dirPath,
		std::ostream & _stream
	) const;

	void dumpIncludes(
		const model_includes::File & _file,
		const Path & _dirPath,
		std::ostream & _stream
	) const;

	void dumpIncludedBy(
		const model_includes::File & _file,
		const Path & _dirPath,
		std::ostream & _stream
	) const;

	void dumpFileFromInclude(
		const model_includes::File & _file,
		const Path & _dirPath,
		const model_includes::Include & _include,
		size_t _index,
		std::ostream & _stream
	) const;

	std::ostream & indent( int _count, std::ostream & _stream ) const;
	std::string toString(
		const model_includes::File & _file,
		const Path & _dirPath
	) const;

	std::string toString( model_includes::FileType _type ) const;
	std::string toString( model_includes::IncludeType _type ) const;
	std::string toString( model_includes::IncludeStatus _status ) const;
};

//------------------------------------------------------------------------------

}
