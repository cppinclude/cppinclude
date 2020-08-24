#include "reporter/impl/rp_dump_reporter.hpp"

#include "reporter/api/enums/rp_reporter_kind.hpp"
#include "reporter/resources/rp_dump_resources.hpp"

#include "model_includes/api/mi_model.hpp"
#include "model_includes/api/mi_file.hpp"
#include "model_includes/api/mi_include.hpp"
#include "model_includes/api/enums/mi_include_status.hpp"
#include "model_includes/api/enums/mi_include_type.hpp"
#include "model_includes/api/enums/mi_file_type.hpp"

#include "exception/ih/exc_internal_error.hpp"

#include <functional>
#include <filesystem>

//------------------------------------------------------------------------------

namespace reporter {

//------------------------------------------------------------------------------

bool DumpReporter::FileSorter::operator()(
	const model_includes::File * _r,
	const model_includes::File * _l
) const
{
	INTERNAL_CHECK_ERROR( _r );
	INTERNAL_CHECK_ERROR( _l );
	return operator()( *_r, *_l );
}

//------------------------------------------------------------------------------

bool DumpReporter::FileSorter::operator()(
	const model_includes::File & _r,
	const model_includes::File & _l
) const
{
	return _r.getPath() < _l.getPath();
}

//------------------------------------------------------------------------------

void DumpReporter::report(
	const model_includes::Model & _model,
	std::ostream & _stream
)
{
	SortedFilesContainer files;
	collectFiles( _model, files );
	dump( files, _model.getProjectDir(), _stream );
}

//------------------------------------------------------------------------------

ReporterKind DumpReporter::getKind() const
{
	return ReporterKind::Dump;
}

//------------------------------------------------------------------------------

void DumpReporter::collectFiles(
	const model_includes::Model & _model,
	SortedFilesContainer & _files
) const
{
	_model.forEachFile( [&]( const model_includes::File & _file )
		{
			_files.insert( &_file );
			return true;
		}
	);
}

//------------------------------------------------------------------------------

void DumpReporter::dump(
	const SortedFilesContainer & _files,
	const std::filesystem::path & _dirPath,
	std::ostream & _stream
) const
{
	using namespace model_includes;

	int number = 1;

	for( const File * filePtr : _files )
	{
		INTERNAL_CHECK_WARRING( filePtr );
		if( !filePtr )
			continue;

		const File & file = *filePtr;
		_stream
			<< number
			<< " : "
			<< toString( file, _dirPath )
			<< " ( type: "
			<< toString( file.getType() )
			<< " )\n"
		;

		dumpIncludes( file, _dirPath, _stream );
		dumpIncludedBy( file, _dirPath, _stream );

		++number;
	}
}

//------------------------------------------------------------------------------

void DumpReporter::dumpIncludes(
	const model_includes::File & _file,
	const std::filesystem::path & _dirPath,
	std::ostream & _stream
) const
{
	using namespace model_includes;

	const File::IncludeIndex count = _file.getIncludesCount();
	if( !count )
		return;

	indent( 1, _stream ) << "Includes:\n";
	for( File::IncludeIndex i = 0; i < count ; ++i )
	{
		const Include & include = _file.getInclude( i );

		dumpFileFromInclude(
			include.getDestinationFile(),
			_dirPath,
			include,
			i,
			_stream
		);
	}
}

//------------------------------------------------------------------------------

void DumpReporter::dumpIncludedBy(
	const model_includes::File & _file,
	const std::filesystem::path & _dirPath,
	std::ostream & _stream
) const
{
	using namespace model_includes;

	const File::IncludeIndex count = _file.getIncludedByCount();
	if( !count )
		return;

	indent( 1, _stream ) << "Included by:\n";
	for( File::IncludeIndex i = 0; i < count; ++i )
	{
		const Include & include = _file.getIncludedBy( i );

		dumpFileFromInclude(
			include.getSourceFile(),
			_dirPath,
			include,
			i,
			_stream
		);
	}
}

//------------------------------------------------------------------------------

void DumpReporter::dumpFileFromInclude(
	const model_includes::File & _file,
	const std::filesystem::path & _dirPath,
	const model_includes::Include & _include,
	size_t _index,
	std::ostream & _stream
) const
{
	indent( 2, _stream )
		<< _index + 1
		<< " : "
		<< toString( _file, _dirPath )
		<< " ( type : "
		<< toString( _include.getType() )
		<< " status : "
		<< toString( _include.getStatus() )
		<< " )\n"
	;
}

//------------------------------------------------------------------------------


std::ostream & DumpReporter::indent( int _count, std::ostream & _stream ) const
{
	for( int i = 0; i < _count; ++i )
		_stream << resources::dump_reporter::Indent;

	return _stream;
}

//------------------------------------------------------------------------------

std::string DumpReporter::toString(
	const model_includes::File & _file,
	const std::filesystem::path & _dirPath
) const
{
	return getPathWithoutProject( _file.getPath(), _dirPath );
}

//------------------------------------------------------------------------------

std::string DumpReporter::toString( model_includes::FileType _fileType ) const
{
	using namespace model_includes;

	static_assert( static_cast< int >( FileType::Count ) == 2 );
	switch( _fileType )
	{
		case FileType::ProjectFile		: return "project file";
		case FileType::StdLibraryFile	: return "standard library file";
		default:
			INTERNAL_CHECK_WARRING( false );
			return "";
	}
}

//------------------------------------------------------------------------------

std::string DumpReporter::toString( model_includes::IncludeType _type ) const
{
	using namespace model_includes;

	static_assert( static_cast< int >( IncludeType::Count ) == 2 );
	switch( _type )
	{
		case IncludeType::User		: return "user include";
		case IncludeType::System	: return "system include";
		default:
			INTERNAL_CHECK_WARRING( false );
			return "";
	}
}

//------------------------------------------------------------------------------

std::string DumpReporter::toString( model_includes::IncludeStatus _status ) const
{
	using namespace model_includes;

	static_assert( static_cast< int >( IncludeStatus::Count )  == 2 );
	switch( _status )
	{
		case IncludeStatus::Resolved	: return "resolved";
		case IncludeStatus::Unresolved	: return "unresolved";
		default:
			INTERNAL_CHECK_WARRING( false );
			return "";
	}
}

//------------------------------------------------------------------------------

}
