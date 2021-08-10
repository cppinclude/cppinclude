#include "reporter/impl/dump_reporter/rp_dump_reporter.hpp"

#include "reporter/api/enums/rp_reporter_kind.hpp"
#include "reporter/resources/rp_dump_resources.hpp"

#include "model_includes/api/enums/mi_file_type.hpp"
#include "model_includes/api/enums/mi_include_status.hpp"
#include "model_includes/api/enums/mi_include_type.hpp"
#include "model_includes/api/mi_file.hpp"
#include "model_includes/api/mi_include.hpp"
#include "model_includes/api/mi_model.hpp"

#include "exception/ih/exc_internal_error.hpp"

#include <functional>
#include <std_fs>

//------------------------------------------------------------------------------

namespace reporter
{
//------------------------------------------------------------------------------

DumpReporter::DumpReporter( SettingsPtr && _settingsPtr )
	: BaseClass{ std::move( _settingsPtr ) }
{
}

//------------------------------------------------------------------------------

void DumpReporter::report(
	const model_includes::Model & _model, std::ostream & _stream )
{
	Files files;
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
	const model_includes::Model & _model, Files & _files ) const
{
	_model.forEachFile( [&]( const model_includes::File & _file ) {
		_files.insert( _file );
		return true;
	} );
}

//------------------------------------------------------------------------------

void DumpReporter::dump(
	const Files & _files, const Path & _dirPath, std::ostream & _stream ) const
{
	using namespace model_includes;

	int number = 1;

	_files.forEachFile( [&]( const File & _file ) {
		_stream << number << " : " << toString( _file, _dirPath )
				<< " ( type: " << toString( _file.getType() ) << " )\n";

		dumpIncludes( _file, _dirPath, _stream );
		dumpIncludedBy( _file, _dirPath, _stream );

		++number;

		return true;
	} );
}

//------------------------------------------------------------------------------

void DumpReporter::dumpIncludes(
	const model_includes::File & _file,
	const Path & _dirPath,
	std::ostream & _stream ) const
{
	using namespace model_includes;

	const File::IncludeIndex count = _file.getIncludesCount();
	if( count == 0U )
	{
		return;
	}

	indent( 1, _stream ) << "Includes:\n";
	for( File::IncludeIndex i = 0; i < count; ++i )
	{
		const Include & include = _file.getInclude( i );

		dumpFileFromInclude(
			include.getDestinationFile(), _dirPath, include, i, _stream );
	}
}

//------------------------------------------------------------------------------

void DumpReporter::dumpIncludedBy(
	const model_includes::File & _file,
	const Path & _dirPath,
	std::ostream & _stream ) const
{
	using namespace model_includes;

	const File::IncludeIndex count = _file.getIncludedByCount();
	if( count == 0U )
	{
		return;
	}

	indent( 1, _stream ) << "Included by:\n";
	for( File::IncludeIndex i = 0; i < count; ++i )
	{
		const Include & include = _file.getIncludedBy( i );

		dumpFileFromInclude(
			include.getSourceFile(), _dirPath, include, i, _stream );
	}
}

//------------------------------------------------------------------------------

void DumpReporter::dumpFileFromInclude(
	const model_includes::File & _file,
	const Path & _dirPath,
	const model_includes::Include & _include,
	size_t _index,
	std::ostream & _stream ) const
{
	indent( 2, _stream ) << _index + 1 << " : " << toString( _file, _dirPath )
						 << " ( type : " << toString( _include.getType() )
						 << " status : " << toString( _include.getStatus() )
						 << " )\n";
}

//------------------------------------------------------------------------------

std::ostream & DumpReporter::indent( int _count, std::ostream & _stream ) const
{
	for( int i = 0; i < _count; ++i )
	{
		_stream << resources::dump_reporter::Indent;
	}

	return _stream;
}

//------------------------------------------------------------------------------

std::string DumpReporter::toString(
	const model_includes::File & _file, const Path & _dirPath ) const
{
	return getPathWithoutProject( _file.getPath(), _dirPath );
}

//------------------------------------------------------------------------------

std::string DumpReporter::toString( model_includes::FileType _type ) const
{
	using namespace model_includes;

	static_assert( static_cast< int >( FileType::Count ) == 2 );
	switch( _type )
	{
		case FileType::ProjectFile:
			return "project file";
		case FileType::StdLibraryFile:
			return "standard library file";
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
		case IncludeType::User:
			return "user include";
		case IncludeType::System:
			return "system include";
		default:
			INTERNAL_CHECK_WARRING( false );
			return "";
	}
}

//------------------------------------------------------------------------------

std::string
DumpReporter::toString( model_includes::IncludeStatus _status ) const
{
	using namespace model_includes;

	static_assert( static_cast< int >( IncludeStatus::Count ) == 2 );
	switch( _status )
	{
		case IncludeStatus::Resolved:
			return "resolved";
		case IncludeStatus::Unresolved:
			return "unresolved";
		default:
			INTERNAL_CHECK_WARRING( false );
			return "";
	}
}

//------------------------------------------------------------------------------

}
