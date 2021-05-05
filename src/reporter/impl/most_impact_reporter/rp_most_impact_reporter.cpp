#include "reporter/impl/most_impact_reporter/rp_most_impact_reporter.hpp"

#include "reporter/api/enums/rp_reporter_kind.hpp"
#include "reporter/impl/most_impact_reporter/rp_most_impact_reporter_detail.hpp"
#include "reporter/impl/tools/rp_file_with_count.hpp"
#include "reporter/impl/tools/rp_file_with_count_container.hpp"
#include "reporter/impl/tools/rp_file_with_count_sorter.hpp"
#include "reporter/resources/rp_most_impact_report_resources.hpp"

#include "model_includes/api/enums/mi_file_type.hpp"
#include "model_includes/api/mi_file.hpp"
#include "model_includes/api/mi_include.hpp"
#include "model_includes/api/mi_include_location.hpp"
#include "model_includes/api/mi_model.hpp"

#include "exception/ih/exc_internal_error.hpp"

#include <fmt/format.h>

#include <functional>
#include <std_fs>

//------------------------------------------------------------------------------

namespace reporter {

//------------------------------------------------------------------------------

bool MostImpcatReporter::DetailsSorter::operator()(
	const MostImpactReporterDetail & _l,
	const MostImpactReporterDetail & _r
) const
{
	return FileWithCountSorter()( _l.getFileInfo(), _r.getFileInfo() );
}

//------------------------------------------------------------------------------

MostImpcatReporter::MostImpcatReporter( SettingsPtr && _settingsPtr )
	:	BaseClass{ std::move( _settingsPtr ) }
{
}

//------------------------------------------------------------------------------

void MostImpcatReporter::report(
	const model_includes::Model & _model,
	std::ostream & _stream
)
{
	FilesContainer files;
	collectFiles( _model, files );

	const size_t originSize = files.getSize();

	files.resizeByLimit( getMaxFilesCount() );
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
			if( isCollectFile( _file ) )
			{
				const File::IncludeIndex count = _file.getIncludedByFilesCountRecursive();
				if( count > 0)
				{
					_files.insert( { _file, count } );
				}
			}

			return true;
		}
	);
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

	if( _files.isEmpty() )
	{
		return;
	}

	_stream << resources::most_impact_report::Header;

	std::size_t currentNumber = 1;

	for( const FileWithCount & fileInfo : _files )
	{
		const File & includedByFile = fileInfo.getFile();
		const Path & includedByPath = includedByFile.getPath();

		const std::string path = getPathWithoutProject( includedByPath, _projectDir );

		const File::IncludeIndex count = fileInfo.getCount();

		_stream << fmt::format(
			resources::most_impact_report::LineForFileFmt,
			currentNumber,
			path,
			count
		);

		printDetails( includedByFile, _projectDir, _stream );

		++currentNumber;
	}

	if( isLimitFilesWithOriginSize( currentNumber, _originSize ) )
	{
		printFileLimitLine( _originSize, _stream );
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
	{
		return;
	}

	_stream << resources::most_impact_report::HeaderForDetails;

	CountType currentNumber = 1;

	for( const MostImpactReporterDetail & detail: details )
	{
		if( isLimitDetails( currentNumber ) )
		{
			printDetailsLimitLine( details.size(), _stream );
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

	if( count > 0 )
	{
		_stream << fmt::format(
			resources::most_impact_report::LineForDetailFmt,
			currentNumber,
			path,
			line,
			count
		);
	}
	else
	{
		_stream << fmt::format(
			resources::most_impact_report::LineForNotImpactDetailFmt,
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
		case FileType::ProjectFile:
			return !getShowOnlyStdHeaders();
		case FileType::StdLibraryFile:
			return getShowStdFiles();
		default:
			INTERNAL_CHECK_WARRING( false );
			return false;
	}
}

//------------------------------------------------------------------------------

}
