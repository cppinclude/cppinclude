#pragma once

#include "reporter/impl/rp_base_reporter_impl.hpp"

#include <set>

//------------------------------------------------------------------------------

namespace model_includes {
	class File;
	class IncludeLocation;
}

//------------------------------------------------------------------------------

namespace reporter {
	class FileInfo;
	class MostImpactReporterDetail;

//------------------------------------------------------------------------------

class MostImpcatReporter final : public BaseReporterImpl
{
public:

	void report(
		const model_includes::Model & _model,
		std::ostream & _stream
	) override;

	ReporterKind getKind() const override;

private:

	class FileWithCountSorter
	{
	public:
		bool operator()( const FileInfo & _r, const FileInfo & _l ) const;
	};

	class DetailsSorter
	{
	public:
		bool operator()(
			const MostImpactReporterDetail & _r,
			const MostImpactReporterDetail & _l
		) const;
	};

	using FilesContainer	= std::set< FileInfo, FileWithCountSorter >;
	using DetailsContainer	= std::set< MostImpactReporterDetail, DetailsSorter >;

	void collectFiles(
		const model_includes::Model & _model,
		FilesContainer & _files
	) const;

	void resizeByLimit( FilesContainer & _files ) const;

	void addFileToContainer(
		const model_includes::File & _file,
		FilesContainer & _files
	) const;

	void printIncludesByFiles(
		const FilesContainer & _files,
		size_t _originSize,
		const Path & _projectDir,
		std::ostream & _stream
	) const;

	void printDetails(
		const model_includes::File & _includedByFile,
		const Path & _projectDir,
		std::ostream & _stream
	) const;

	void printDetail(
		const MostImpactReporterDetail & _detail,
		const Path & _projectDir,
		size_t currentNumber,
		std::ostream & _stream
	) const;

	void collectDetails(
		const model_includes::File & _includedByFile,
		DetailsContainer & _details
	) const;

	bool isCollectFile( const model_includes::File & _file ) const;

	static FileInfo makeFileInfo( const model_includes::File & _file );
};

//------------------------------------------------------------------------------

}
