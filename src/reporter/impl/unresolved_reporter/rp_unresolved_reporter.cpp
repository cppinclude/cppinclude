#include "reporter/impl/unresolved_reporter/rp_unresolved_reporter.hpp"

#include "reporter/api/enums/rp_reporter_kind.hpp"
#include "reporter/resources/rp_unresolved_report_resources.hpp"
#include "reporter/resources/rp_report_resources.hpp"
#include "reporter/impl/tools/rp_file_sorter.hpp"
#include "reporter/impl/tools/rp_includes_by_source_sorter.hpp"
#include "reporter/impl/tools/rp_file_with_count_container.hpp"
#include "reporter/impl/tools/rp_file_with_count.hpp"

#include "model_includes/api/mi_model.hpp"
#include "model_includes/api/mi_file.hpp"
#include "model_includes/api/mi_include.hpp"
#include "model_includes/api/mi_include_location.hpp"
#include "model_includes/api/enums/mi_include_status.hpp"

#include "exception/ih/exc_internal_error.hpp"

#include <fmt/format.h>

#include <set>
#include <functional>
#include <std_fs>
#include <unordered_map>
#include <vector>

//------------------------------------------------------------------------------

namespace reporter {

//------------------------------------------------------------------------------

UnresolvedReporter::UnresolvedReporter( SettingsPtr && _settingsPtr )
	:	BaseClass{ std::move( _settingsPtr ) }
{

}

//------------------------------------------------------------------------------

void UnresolvedReporter::report(
	const model_includes::Model & _model,
	std::ostream & _stream
)
{
	UnorderedIncludesByDestination unorderedIncludes;
	collectIncludes( _model, unorderedIncludes );

	DestinationFileByCount destinationFilesByCount =
		orderDestinationByCount( unorderedIncludes );

	report(
		unorderedIncludes,
		destinationFilesByCount,
		_model.getProjectDir(),
		_stream
	);
}

//------------------------------------------------------------------------------

ReporterKind UnresolvedReporter::getKind() const
{
	return ReporterKind::Unresolved;
}

//------------------------------------------------------------------------------

void UnresolvedReporter::collectIncludes(
	const model_includes::Model & _model,
	UnorderedIncludesByDestination & _unorderedIncludes
) const
{
	using namespace model_includes;

	_model.forEachInclude(
		[&]( const Include & _include )
		{
			if( isUnresolvedInclude( _include  ) )
			{
				const File & destinationFile = _include.getDestinationFile();
				_unorderedIncludes[ &destinationFile ].push_back( &_include );
			}

			return true;
		}
	);
}

//------------------------------------------------------------------------------

UnresolvedReporter::DestinationFileByCount UnresolvedReporter::orderDestinationByCount(
	const UnorderedIncludesByDestination & _unorderedIncludes
)
{
	DestinationFileByCount result;
	for( const auto & pair : _unorderedIncludes )
	{
		const model_includes::File * destinationFile = pair.first;
		const auto & includes = pair.second;

		result.insert( { *destinationFile, includes.size() } );
	}
	return result;
}

//------------------------------------------------------------------------------

void UnresolvedReporter::report(
	const UnorderedIncludesByDestination & _unorderedIncludesByDestination,
	const DestinationFileByCount & _destinationFileByCount,
	const Path & _projectDir,
	std::ostream & _stream
) const
{
	using namespace model_includes;

	if( _unorderedIncludesByDestination.empty() )
		return;

	_stream << resources::unresolved_report::Header;

	CountType currentNumber = 1;
	for( const FileWithCount & info : _destinationFileByCount )
	{
		if( isLimitFiles( currentNumber ) )
		{
			const size_t filesCount = _destinationFileByCount.getSize();
			printFileLimitLine( filesCount, _stream );
			break;
		}

		const File & destinationFile = info.getFile();

		reportDestinationFile( destinationFile, currentNumber, _projectDir, _stream );

		if( !_unorderedIncludesByDestination.count( &destinationFile ) )
		{
			INTERNAL_CHECK_WARRING( false );
			continue;
		}
		const UnorderedIncludes & includes =
			_unorderedIncludesByDestination.at( &destinationFile );

		reportSourceFiles( includes, _projectDir, _stream );

		++currentNumber;

	}
}

//------------------------------------------------------------------------------

void UnresolvedReporter::reportDestinationFile(
	const model_includes::File & _file,
	size_t _number,
	const Path & _projectDir,
	std::ostream & _stream
) const
{
	const std::string destinationPath =
		getPathWithoutProject( _file.getPath(), _projectDir );

	INTERNAL_CHECK_WARRING( !destinationPath.empty() );

	_stream << fmt::format(
		resources::unresolved_report::UnresolvedDestinationFmt,
		_number,
		destinationPath
	);
}

//------------------------------------------------------------------------------

void UnresolvedReporter::reportSourceFiles(
	const UnorderedIncludes & _includes,
	const Path & _projectDir,
	std::ostream & _stream
) const
{
	OrderedIncludes orderedIncludes{ _includes.begin(), _includes.end() };
	CountType currentNumber = 1;
	for( const model_includes::Include * includePtr : orderedIncludes )
	{
		if( isLimitDetails( currentNumber ) )
		{
			printDetailsLimitLine( orderedIncludes.size(), _stream );
			break;
		}

		INTERNAL_CHECK_WARRING( includePtr );
		if( !includePtr )
			continue;
		reportSourceFile( *includePtr, currentNumber, _projectDir, _stream );

		++currentNumber;
	}
}

//------------------------------------------------------------------------------

void UnresolvedReporter::reportSourceFile(
	const model_includes::Include & _include,
	size_t _number,
	const Path & _projectDir,
	std::ostream & _stream
) const
{
	using namespace model_includes;

	_stream << resources::unresolved_report::Intend;

	const File & sourceFile = _include.getSourceFile();
	const IncludeLocation & location = _include.getLocation();
	const IncludeLocation::LineNumber line = location.getLineNumber();

	const std::string sourcePath =
		getPathWithoutProject( sourceFile.getPath(), _projectDir );

	INTERNAL_CHECK_WARRING( !sourcePath.empty() );

	_stream << fmt::format(
		resources::unresolved_report::UnresolvedSourceFmt,
		_number,
		sourcePath,
		line
	);
}

//------------------------------------------------------------------------------

bool UnresolvedReporter::isUnresolvedInclude(
	const model_includes::Include & _include
) const
{
	using namespace model_includes;

	const IncludeStatus status = _include.getStatus();
	static_assert( static_cast< int >( IncludeStatus::Count ) == 2 );
	switch( status )
	{
		case IncludeStatus::Resolved	: return false;
		case IncludeStatus::Unresolved	: return true;
		default:
			INTERNAL_CHECK_WARRING( false );
			return false;
	}
}

//------------------------------------------------------------------------------

}
