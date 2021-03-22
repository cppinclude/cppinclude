#include "reporter/impl/unincluded_reporter/rp_unincluded_reporter.hpp"

#include "reporter/api/enums/rp_reporter_kind.hpp"
#include "reporter/impl/tools/rp_sorted_files_by_name_container.hpp"
#include "reporter/resources/rp_unincluded_report_resources.hpp"

#include "model_includes/api/mi_file.hpp"
#include "model_includes/api/mi_model.hpp"

#include <fmt/format.h>
#include <std_fs>

#include <functional>
#include <set>

//------------------------------------------------------------------------------

namespace reporter {

//------------------------------------------------------------------------------

UnincludedReporter::UnincludedReporter( SettingsPtr && _settingsPtr )
	:	BaseClass{ std::move( _settingsPtr ) }
{
}

//------------------------------------------------------------------------------

void UnincludedReporter::report(
	const model_includes::Model & _model,
	std::ostream & _stream
)
{
	FilesContainer files = collectFiles( _model );
	printFiles( files, _model.getProjectDir(), _stream );
}

//------------------------------------------------------------------------------

UnincludedReporter::FilesContainer UnincludedReporter::collectFiles(
	const model_includes::Model & _model
) const
{
	FilesContainer files;
	_model.forEachFile( [&]( const model_includes::File & _file )
	{
		if( isHeader( _file ) && _file.getIncludedByCount() == 0u )
		{
			files.insert( _file );
		}
		return true;
	});

	return files;
}

//------------------------------------------------------------------------------

ReporterKind UnincludedReporter::getKind() const
{
	return ReporterKind::Unincluded;
}

//------------------------------------------------------------------------------

bool UnincludedReporter::isHeader( const model_includes::File & _file ) const
{
	const std::string extensionStr = _file.getPath().extension().string();

	if( extensionStr.empty() )
	{
		return true;
	}

	if( extensionStr.size() > 1 )
	{
		const bool result = extensionStr.at( 1 ) == 'h';
		return result;
	}
	return false;
}

//------------------------------------------------------------------------------

void UnincludedReporter::printFiles(
	const FilesContainer & _files,
	const Path & _projectDir,
	std::ostream & _stream
) const
{
	if( _files.isEmpty() )
	{
		return;
	}

	_stream << resources::unincluded_report::Header;

	CountType currentNumber = 1;
	_files.forEachFile(
		[&]( const model_includes::File & _file )
		{
			if( isLimitFiles( currentNumber ) )
			{
				const size_t filesCount = _files.getSize();
				printFileLimitLine( filesCount, _stream );
				return false;
			}

			const std::string path = getPathWithoutProject(
				_file.getPath(),
				_projectDir
			);

			_stream << fmt::format(
				resources::unincluded_report::LineForFileFmt,
				currentNumber,
				path
			);

			++currentNumber;

			return true;
		}
	);
}

//------------------------------------------------------------------------------

}
