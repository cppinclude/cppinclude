#pragma once

#include "reporter/impl/rp_base_reporter_impl.hpp"

#include <stdfwd.hpp>

//------------------------------------------------------------------------------

namespace model_includes {
	class File;
}

//------------------------------------------------------------------------------

namespace reporter {
	class SortedFilesByNameContainer;

//------------------------------------------------------------------------------

class UnincludedReporter final : public BaseReporterImpl
{
	using BaseClass = BaseReporterImpl;

public:
	explicit UnincludedReporter( SettingsPtr && _settingsPtr );

	void report(
		const model_includes::Model & _model,
		std::ostream & _stream
	) override;

	ReporterKind getKind() const override;

private:
	using Path = stdfs::path;

	using FilesContainer = SortedFilesByNameContainer;

	FilesContainer collectFiles( const model_includes::Model & _model ) const;
	bool isHeader( const model_includes::File & _file ) const;

	void printFiles(
		const FilesContainer & _files,
		const Path & _projectDir,
		std::ostream & _stream
	) const;
};

//------------------------------------------------------------------------------

}
