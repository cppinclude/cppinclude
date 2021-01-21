#include "reporter/impl/different_type_reporter/rp_different_type_reporter.hpp"

#include "reporter/api/enums/rp_reporter_kind.hpp"
#include "reporter/impl/different_type_reporter/rp_different_type_reporter_details.hpp"
#include "reporter/impl/tools/rp_file_with_count_container.hpp"
#include "reporter/impl/tools/rp_sorted_includes_by_source_container.hpp"
#include "reporter/resources/rp_different_type_report_resources.hpp"

#include "model_includes/api/mi_model.hpp"
#include "model_includes/api/mi_file.hpp"
#include "model_includes/api/mi_include.hpp"
#include "model_includes/api/mi_include_location.hpp"
#include "model_includes/api/enums/mi_file_type.hpp"

#include "exception/ih/exc_internal_error.hpp"

#include <fmt/format.h>

#include <functional>
#include <ostream>
#include <optional>

//------------------------------------------------------------------------------

namespace reporter {

//------------------------------------------------------------------------------

DifferentTypeReporter::DifferentTypeReporter( SettingsPtr && _settingsPtr )
	:	BaseClass{ std::move( _settingsPtr ) }
{

}

//------------------------------------------------------------------------------

void DifferentTypeReporter::report(
	const model_includes::Model & _model,
	std::ostream & _stream
)
{
	Files files = collectFiles( _model );

	const std::size_t originSize = files.getSize();
	files.resizeByLimit( getMaxFilesCount() );

	printFiles( files, _model.getProjectDir(), originSize, _stream );
}

//------------------------------------------------------------------------------

void DifferentTypeReporter::printFiles(
	const Files & _files,
	const Path & _projectDir,
	CountType _originSize,
	std::ostream & _stream
) const
{
	if( _files.isEmpty() )
		return;

	_stream << resources::different_type_report::Header;

	CountType currentNumber = 1;
	_files.forEachFile( [&]( const File & _file )
	{
		if( isLimitFiles( currentNumber ) )
		{
			printFileLimitLine( _files.getSize(), _stream );
			return false;
		}

		printFile( _file, _projectDir, currentNumber, _stream );

		++currentNumber;

		return true;
	});

	if( isLimitFilesWithOriginSize( currentNumber, _originSize ) )
		printFileLimitLine( _originSize, _stream );

}

//------------------------------------------------------------------------------

void DifferentTypeReporter::printFile(
	const File & _file,
	const Path & _projectDir,
	CountType _currentNumber,
	std::ostream & _stream
) const
{
	std::string filePath = getPathWithoutProject( _file.getPath(), _projectDir );

	_stream << fmt::format(
		resources::different_type_report::FileFmt,
		_currentNumber,
		filePath
	);

	Details details = collectDetails( _file );
	printDetails( details, _projectDir, _stream );
}

//------------------------------------------------------------------------------

void DifferentTypeReporter::printDetails(
	const Details & _details,
	const Path & _projectDir,
	std::ostream & _stream
) const
{
	printDetails(
		_details.getUserIncludes(),
		resources::different_type_report::UserIncludeLabel,
		_projectDir,
		_stream
	);

	printDetails(
		_details.getSystemIncludes(),
		resources::different_type_report::SystemIncludeLabel,
		_projectDir,
		_stream
	);
}

//------------------------------------------------------------------------------

void DifferentTypeReporter::printDetails(
	const Includes & _details,
	std::string_view _header,
	const Path & _projectDir,
	std::ostream & _stream
) const
{
	_stream << _header;
	CountType currentUserNumber = 1;

	_details.forEachInclude(
		[&]( const Include & _include )
		{
			if( isLimitDetails( currentUserNumber ) )
			{
				printDetailsLimitLine( _details.getCount(), _stream );
				return false;
			}

			const File & file = _include.getSourceFile();
			const auto lineNumber = _include.getLocation().getLineNumber();
			const auto filePath = getPathWithoutProject(
				file.getPath(),
				_projectDir
			);

			_stream << fmt::format(
				resources::different_type_report::DetailFmt,
				currentUserNumber,
				filePath,
				lineNumber
			);

			++currentUserNumber;

			return true;
		}
	);
}

//------------------------------------------------------------------------------

ReporterKind DifferentTypeReporter::getKind() const
{
	return ReporterKind::DifferentType;
}

//------------------------------------------------------------------------------

DifferentTypeReporter::Files DifferentTypeReporter::collectFiles(
	const model_includes::Model & _model
) const
{
	Files result;
	_model.forEachFile(
		[&]( const model_includes::File & _file )
		{
			if( isIgnoredFile( _file ) )
				return true;

			if( isIncludedByDifferentType( _file ) )
				result.insert( { _file, _file.getIncludedByCount() } );

			return true;
		}
	);
	return result;
}

//------------------------------------------------------------------------------

DifferentTypeReporter::Details DifferentTypeReporter::collectDetails(
	const File & _file
) const
{
	Details result;
	for( File::IncludeIndex i = 0; i < _file.getIncludedByCount(); ++i )
	{
		const Include & include = _file.getIncludedBy( i );
		result.insertInclude( include );
	}
	return result;
}

//------------------------------------------------------------------------------

bool DifferentTypeReporter::isIncludedByDifferentType( const File & _file )
{
	if( _file.getIncludedByCount() < 2)
		return false;

	const auto firstType = _file.getIncludedBy( 0 ).getType();

	for( File::IncludeIndex i = 1; i < _file.getIncludedByCount(); ++i )
	{
		const auto currentType = _file.getIncludedBy( i ).getType();
		if( firstType != currentType )
			return true;
	}

	return false;
}

//------------------------------------------------------------------------------

bool DifferentTypeReporter::isIgnoredFile( const File & _file ) const
{
	using namespace model_includes;

	if( getShowStdFiles() )
		return false;

	const FileType type = _file.getType();
	static_assert( static_cast< int >( FileType::Count ) == 2 );
	switch( type )
	{
		case FileType::ProjectFile		: return false;
		case FileType::StdLibraryFile	: return true;
		default:
			INTERNAL_CHECK_WARRING( false );
			return false;
	}
}

//------------------------------------------------------------------------------

}
