#include "reporter/impl/rp_unresolved_reporter.hpp"

#include "reporter/resources/rp_unresolved_reporter_resources.hpp"
#include "reporter/resources/rp_report_resources.hpp"

#include "reporter/api/enums/rp_reporter_kind.hpp"

#include "model_includes/api/mi_model.hpp"
#include "model_includes/api/mi_file.hpp"
#include "model_includes/api/mi_include.hpp"
#include "model_includes/api/mi_include_location.hpp"
#include "model_includes/api/enums/mi_include_status.hpp"

#include "exception/ih/exc_internal_error.hpp"

#include <fmt/format.h>

#include <set>
#include <functional>
#include <filesystem>

//------------------------------------------------------------------------------

namespace reporter {

//------------------------------------------------------------------------------

bool UnresolvedReporter::SourceOrder::operator()(
	const model_includes::Include * _r,
	const model_includes::Include * _l
) const
{
	INTERNAL_CHECK_ERROR( _r );
	INTERNAL_CHECK_ERROR( _l );
	const model_includes::File & sourceRight = _r->getSourceFile();
	const model_includes::File & sourceLeft = _l->getSourceFile();

	return sourceRight.getPath() < sourceLeft.getPath();
}

//------------------------------------------------------------------------------

bool UnresolvedReporter::DestinationFileInfoorder::operator()(
	const DestinationFileInfo & _r,
	const DestinationFileInfo & _l
) const
{
	if( _r.m_count != _l.m_count )
	{
		return _r.m_count > _l.m_count;
	}
	else
	{
		INTERNAL_CHECK_ERROR( _r.m_file );
		INTERNAL_CHECK_ERROR( _l.m_file );
		return _r.m_file->getPath() < _l.m_file->getPath();
	}
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

		auto pairInsert = result.insert( { destinationFile, includes.size() } );
		if( !pairInsert.second )
		{
			INTERNAL_CHECK_WARRING( false );
		}
	}
	return result;
}

//------------------------------------------------------------------------------

void UnresolvedReporter::report(
	const UnorderedIncludesByDestination & _unorderedIncludesByDestination,
	const DestinationFileByCount & _destinationFileByCount,
	const std::filesystem::path & _projectDir,
	std::ostream & _stream
) const
{
	using namespace model_includes;

	if( _unorderedIncludesByDestination.empty() )
		return;

	_stream << resources::unresolved_reporter::Header;

	size_t number = 1;
	const size_t limit = getMaxFilesCount();
	for( const DestinationFileInfo & info : _destinationFileByCount )
	{
		if( limit && number > limit )
		{
			const size_t detailsCount = _destinationFileByCount.size();
			_stream << fmt::format( resources::LimitLineFmt, limit, detailsCount );
			break;
		}

		const File * destinationFilePtr = info.m_file;
		INTERNAL_CHECK_WARRING( destinationFilePtr );
		if( !destinationFilePtr )
			continue;

		const File & destinationFile = *destinationFilePtr;

		reportDestinationFile( destinationFile, number, _projectDir, _stream );

		if( !_unorderedIncludesByDestination.count( destinationFilePtr ) )
		{
			INTERNAL_CHECK_WARRING( false );
			continue;
		}
		const UnorderedIncludes & includes =
			_unorderedIncludesByDestination.at( destinationFilePtr );

		reportSourceFiles( includes, _projectDir, _stream );

		++number;
	}
}

//------------------------------------------------------------------------------

void UnresolvedReporter::reportDestinationFile(
	const model_includes::File & _file,
	size_t _number,
	const std::filesystem::path & _projectDir,
	std::ostream & _stream
) const
{
	const std::string destinationPath =
		getPathWithoutProject( _file.getPath(), _projectDir );

	INTERNAL_CHECK_WARRING( !destinationPath.empty() );

	_stream << fmt::format(
		resources::unresolved_reporter::UnresolvedDestinationFmt,
		_number,
		destinationPath
	);
}

//------------------------------------------------------------------------------

void UnresolvedReporter::reportSourceFiles(
	const UnorderedIncludes & _includes,
	const std::filesystem::path & _projectDir,
	std::ostream & _stream
) const
{
	OrderedIncludes orderedIncludes{ _includes.begin(), _includes.end() };
	size_t number = 1;
	const size_t limit = getMaxDetailsCount();
	for( const model_includes::Include * includePtr : orderedIncludes )
	{
		if( limit && number > limit )
		{
			_stream << fmt::format(
				resources::LimitDetailLineFmt,
				limit,
				orderedIncludes.size()
			);
			break;
		}

		INTERNAL_CHECK_WARRING( includePtr );
		if( !includePtr )
			continue;
		reportSourceFile( *includePtr, number, _projectDir, _stream );

		++number;
	}
}

//------------------------------------------------------------------------------

void UnresolvedReporter::reportSourceFile(
	const model_includes::Include & _include,
	size_t _number,
	const std::filesystem::path & _projectDir,
	std::ostream & _stream
) const
{
	using namespace model_includes;

	_stream << resources::unresolved_reporter::Intend;

	const File & sourceFile = _include.getSourceFile();
	const IncludeLocation & location = _include.getLocation();
	const IncludeLocation::LineNumber line = location.getLineNumber();

	const std::string sourcePath =
		getPathWithoutProject( sourceFile.getPath(), _projectDir );

	INTERNAL_CHECK_WARRING( !sourcePath.empty() );

	_stream << fmt::format(
		resources::unresolved_reporter::UnresolvedSourceFmt,
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
