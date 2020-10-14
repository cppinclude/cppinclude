#include "reporter/impl/rp_most_impact_reporter.hpp"

#include "reporter/api/enums/rp_reporter_kind.hpp"
#include "reporter/impl/impact_reporter/rp_file_info.hpp"
#include "reporter/impl/impact_reporter/rp_most_impact_reporter_detail.hpp"
#include "reporter/resources/rp_most_impact_reporter_resources.hpp"
#include "reporter/resources/rp_report_resources.hpp"

#include "model_includes/api/mi_model.hpp"
#include "model_includes/api/mi_file.hpp"
#include "model_includes/api/mi_include.hpp"
#include "model_includes/api/mi_include_location.hpp"
#include "model_includes/api/enums/mi_file_type.hpp"

#include "exception/ih/exc_internal_error.hpp"

#include <std_fs>
#include <functional>
#include <fmt/format.h>

//------------------------------------------------------------------------------

namespace reporter {

//------------------------------------------------------------------------------

bool MostImpcatReporter::FileWithCountSorter::operator()(
	const FileInfo & _r,
	const FileInfo & _l
) const
{
	if( _r.getIncludedByCount() != _l.getIncludedByCount() )
	{
		// max is first
		return _r.getIncludedByCount() > _l.getIncludedByCount();
	}
	else
	{
		return _r.getFile().getPath() < _l.getFile().getPath();
	}
}

//------------------------------------------------------------------------------

bool MostImpcatReporter::DetailsSorter::operator()(
	const MostImpactReporterDetail & _r,
	const MostImpactReporterDetail & _l
) const
{
	return FileWithCountSorter()( _r.getFileInfo(), _l.getFileInfo() );
}

//------------------------------------------------------------------------------

void MostImpcatReporter::report(
	const model_includes::Model & _model,
	std::ostream & _stream
)
{
	FilesContainer files;
	collectFiles( _model, files );

	const size_t originSize = files.size();

	resizeByLimit( files );
	printIncludesByFiles( files, originSize, _model.getProjectDir(), _stream );
}

//------------------------------------------------------------------------------

ReporterKind MostImpcatReporter::getKind() const
{
	return ReporterKind::MostImpact;
}

//------------------------------------------------------------------------------

void MostImpcatReporter::collectFiles(
	const model_includes::Model & _model,
	FilesContainer & _files
) const
{
	using namespace model_includes;

	_model.forEachFile(
		[&]( const File & _file )
		{
			if( !isCollectFile( _file ) )
				return true;

			const auto count = 	_file.getIncludedByFilesCountRecursive();
			if( count > 0 )
			{
				_files.insert( { _file, count } );
			}
			return true;
		}
	);
}

//------------------------------------------------------------------------------

void MostImpcatReporter::resizeByLimit( FilesContainer & _files ) const
{
	const size_t limit = getMaxFilesCount();
	if( limit && _files.size() >= limit )
	{
		auto it = _files.begin();
		for( size_t i = 0; i < limit; ++i )
			++it;

		_files.erase( it, _files.end() );
	}
}

//------------------------------------------------------------------------------

void MostImpcatReporter::printIncludesByFiles(
	const FilesContainer & _files,
	size_t _originSize,
	const Path & _projectDir,
	std::ostream & _stream
) const
{
	using namespace model_includes;

	if( _files.empty() )
		return;

	_stream << resources::most_impact_reporter::Header;

	size_t currentNumber = 1;

	for( const FileInfo & fileInfo : _files )
	{
		const File & includedByFile = fileInfo.getFile();

		const std::string path =
			getPathWithoutProject( includedByFile.getPath(), _projectDir );

		const File::IncludeIndex count = fileInfo.getIncludedByCount();

		_stream << fmt::format(
			resources::most_impact_reporter::LineForFileFmt,
			currentNumber,
			path,
			count
		);

		printDetails( includedByFile, _projectDir, _stream );

		++currentNumber;
	}
	const size_t limit = getMaxFilesCount();

	if(
		limit &&
		currentNumber >= limit &&
		currentNumber - 1 !=  _originSize
	)
	{
		_stream << fmt::format( resources::LimitLineFmt, limit, _originSize );
	}
}

//------------------------------------------------------------------------------

void MostImpcatReporter::printDetails(
	const model_includes::File & _includedByFile,
	const Path & _projectDir,
	std::ostream & _stream
) const
{
	using namespace model_includes;

	DetailsContainer details;
	collectDetails( _includedByFile, details );

	if( details.empty() )
		return;

	_stream << resources::most_impact_reporter::HeaderForDetails;

	size_t currentNumber = 1;
	const size_t limit = getMaxDetailsCount();

	for( const MostImpactReporterDetail & detail : details )
	{
		if( limit && currentNumber > limit )
		{
			const size_t detailsCount = details.size();
			_stream << fmt::format(
				resources::LimitDetailLineFmt,
				limit,
				detailsCount
			);
			break;
		}

		printDetail( detail, _projectDir, currentNumber, _stream );

		++currentNumber;
	}
}

//------------------------------------------------------------------------------

void MostImpcatReporter::printDetail(
	const MostImpactReporterDetail & _detail,
	const Path & _projectDir,
	size_t currentNumber,
	std::ostream & _stream
) const
{
	using namespace model_includes;

	const File & file = _detail.getFile();

	const std::string path =
		getPathWithoutProject( file.getPath(), _projectDir );

	const File::IncludeIndex count = _detail.getIncludedByCount();

	const auto line = _detail.getIncludeLocation().getLineNumber();

	if( count )
	{
		_stream << fmt::format(
			resources::most_impact_reporter::LineForDetailFmt,
			currentNumber,
			path,
			line,
			count
		);
	}
	else
	{
		_stream << fmt::format(
			resources::most_impact_reporter::LineForNotImpactDetailFmt,
			currentNumber,
			path,
			line
		);
	}

}

//------------------------------------------------------------------------------

void MostImpcatReporter::collectDetails(
	const model_includes::File & _includedByFile,
	DetailsContainer & _details
) const
{
	using namespace model_includes;

	const File::IncludeIndex count = _includedByFile.getIncludedByCount();
	for( File::IncludeIndex i = 0; i < count; ++i )
	{
		const Include & include = _includedByFile.getIncludedBy( i );
		const File & file = include.getSourceFile();
		_details.insert({
			file,
			file.getIncludedByFilesCountRecursive(),
			include.getLocation()
		});
	}
}

//------------------------------------------------------------------------------

bool MostImpcatReporter::isCollectFile( const model_includes::File & _file ) const
{
	using namespace model_includes;

	const FileType type = _file.getType();
	static_assert( static_cast< int >( FileType::Count ) == 2 );
	switch( type )
	{
		case FileType::ProjectFile		: return true;
		case FileType::StdLibraryFile	: return getShowStdFiles();
		default:
			INTERNAL_CHECK_WARRING( false );
			return false;
	}
}

//------------------------------------------------------------------------------

}
