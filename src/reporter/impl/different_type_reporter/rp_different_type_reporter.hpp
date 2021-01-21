#pragma once

#include "reporter/impl/rp_base_reporter_impl.hpp"

//------------------------------------------------------------------------------

namespace model_includes {
	class File;
	class Include;
}

//------------------------------------------------------------------------------

namespace reporter {
	class FileWithCountContainer;
	class DifferentTypeReporterDetails;
	class SortedIncludesBySourceContainer;

//------------------------------------------------------------------------------

class DifferentTypeReporter final : public BaseReporterImpl
{
	using BaseClass = BaseReporterImpl;

public:

	explicit DifferentTypeReporter( SettingsPtr && _settingsPtr );

	void report(
		const model_includes::Model & _model,
		std::ostream & _stream
	) override;

	ReporterKind getKind() const override;

private:

	using File			= model_includes::File;
	using Files			= FileWithCountContainer;
	using Path			= stdfs::path;
	using Details		= DifferentTypeReporterDetails;
	using Include		= model_includes::Include;
	using Includes		= SortedIncludesBySourceContainer;

	Files collectFiles( const model_includes::Model & _model ) const;
	Details collectDetails( const File & _file ) const;

	void printFiles(
		const Files & _files,
		const Path & _projectDir,
		CountType _originSize,
		std::ostream & _stream
	) const;

	void printFile(
		const model_includes::File & _file,
		const Path & _projectDir,
		CountType _currentNumber,
		std::ostream & _stream
	) const;

	void printDetails(
		const Details & _details,
		const Path & _projectDir,
		std::ostream & _stream
	) const;

	void printDetails(
		const Includes & _details,
		std::string_view _header,
		const Path & _projectDir,
		std::ostream & _stream
	) const;

	static bool isIncludedByDifferentType( const File & _file );
	bool isIgnoredFile( const File & _file ) const;

};

//------------------------------------------------------------------------------

}
