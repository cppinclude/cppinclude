#include "reporter/test/fixture/rp_test_reporter_fixture.hpp"

#include "reporter/ih/rp_accessor_impl.hpp"

#include "reporter/api/rp_factory.hpp"
#include "reporter/api/rp_reporter.hpp"
#include "reporter/api/rp_settings.hpp"

#include "model_includes/ih/mi_accessor_impl.hpp"
#include "model_includes/api/mi_model.hpp"
#include "model_includes/api/enums/mi_file_type.hpp"

#include "tools/path_string_tools.hpp"

#include "exception/ih/exc_internal_error.hpp"

#include <string>
#include <sstream>
#include <std_fs>

//------------------------------------------------------------------------------

namespace reporter::test {

//------------------------------------------------------------------------------

ReporterFixture::ReporterFixture() = default;
ReporterFixture::~ReporterFixture() = default;

//------------------------------------------------------------------------------

void ReporterFixture::addInclude(
	std::string_view _sourceFile,
	std::string_view _destinationFile,
	IncludeType _type
)
{
	addInclude(
		_sourceFile,
		_destinationFile,
		IncludeStatus::Resolved,
		_type,
		{1,1,1}
	);
}

//------------------------------------------------------------------------------

void ReporterFixture::addUserInclude(
	std::string_view _sourceFile,
	std::string_view _destinationFile
)
{
	addInclude( _sourceFile, _destinationFile, IncludeType::User );
}

//------------------------------------------------------------------------------

void ReporterFixture::addSystemInclude(
	std::string_view _sourceFile,
	std::string_view _destinationFile
)
{
	addInclude( _sourceFile, _destinationFile, IncludeType::System );
}

//------------------------------------------------------------------------------

void ReporterFixture::addInclude(
	std::string_view _sourceFile,
	std::string_view _destinationFile,
	IncludeStatus _status,
	IncludeType _type,
	const LocationInfo & _location
)
{
	using namespace model_includes;

	File & sourceFile = addFile( _sourceFile );
	File & destinationFile = addFile( _destinationFile );

	Model::IncludeLocationInfo location{
		_location.m_line,
		_location.m_begin,
		_location.m_end
	};

	getModel().createInclude(
		location,
		sourceFile,
		destinationFile,
		_status,
		_type
	);
}

//------------------------------------------------------------------------------

void ReporterFixture::setProjectDir( std::string_view _dir )
{
	getModel().setProjectDir( _dir );
}

//------------------------------------------------------------------------------

ReporterFixture::Path ReporterFixture::getProjectDir()
{
	return getModel().getProjectDir();
}

//------------------------------------------------------------------------------

void ReporterFixture::setMaxFilesCount( int _count )
{
	ensureSettings().setMaxFilesCount( _count );
}

//------------------------------------------------------------------------------

void ReporterFixture::setMaxDetailsCount( int _count )
{
	ensureSettings().setMaxDetailsCount( _count );
}

//------------------------------------------------------------------------------

void ReporterFixture::setShowStdFiles( bool _enable )
{
	ensureSettings().setShowStdFiles( _enable );
}

//------------------------------------------------------------------------------

std::string ReporterFixture::runDumpReporter()
{
	auto reporterPtr = getFactory().createDumpReporter();
	INTERNAL_CHECK_ERROR( reporterPtr );
	return runReporter( *reporterPtr );
}

//------------------------------------------------------------------------------

std::string ReporterFixture::runUnresolvedReporter()
{
	auto reporterPtr = getFactory().createUnresolvedReporter();
	INTERNAL_CHECK_ERROR( reporterPtr );
	return runReporter( *reporterPtr );
}

//------------------------------------------------------------------------------

std::string ReporterFixture::runMostImpactReporter()
{
	auto reporterPtr = getFactory().createMostImpactReporter();
	INTERNAL_CHECK_ERROR( reporterPtr );
	return runReporter( *reporterPtr );
}

//------------------------------------------------------------------------------

std::string ReporterFixture::runUnincludedReporter()
{
	auto reporterPtr = getFactory().createUnincludedReporter();
	INTERNAL_CHECK_ERROR( reporterPtr );
	return runReporter( *reporterPtr );
}

//------------------------------------------------------------------------------

std::string ReporterFixture::runDifferentTypeReport()
{
	auto reporterPtr = getFactory().createDifferentTypeReporter();
	INTERNAL_CHECK_ERROR( reporterPtr );
	return runReporter( *reporterPtr );
}

//------------------------------------------------------------------------------

model_includes::File & ReporterFixture::addFile(
	std::string_view _file,
	FileType _type
)
{
	return getModel().ensureFile( _file, _type );
}

//------------------------------------------------------------------------------

model_includes::File & ReporterFixture::addFileToProject(
	std::string_view _file
)
{
	Path filePah = getProjectDir() / _file;
	return addFile( filePah.string() );
}

//------------------------------------------------------------------------------

std::string ReporterFixture::toPath( std::string_view _str )
{
	return tools::toPath( std::string{ _str } );
}

//------------------------------------------------------------------------------

std::string ReporterFixture::runReporter( Reporter & _reporter )
{
	std::stringstream stream;
	const model_includes::Model & model = getModel();

	const Settings & settings = ensureSettings();
	_reporter.copySettings( settings );
	_reporter.report( model, stream );

	return stream.str();
}

//------------------------------------------------------------------------------

ReporterAccessor & ReporterFixture::ensureReportAccessor()
{
	if( !m_accessor )
		m_accessor.reset( new ReporterAccessorImpl );

	return *m_accessor;
}

//------------------------------------------------------------------------------

Factory & ReporterFixture::getFactory()
{
	return ensureReportAccessor().getReporterFactory();
}

//------------------------------------------------------------------------------

Settings & ReporterFixture::ensureSettings()
{
	if( !m_settings )
	{
		auto newSettings = getFactory().createSettings();
		m_settings.swap( newSettings );
	}

	return *m_settings;
}

//------------------------------------------------------------------------------

model_includes::ModelIncludesAccessor & ReporterFixture::getModelAccessor()
{
	if( !m_modelAccessor )
		m_modelAccessor.reset( new model_includes::ModelIncludesAccessorImpl );

	return *m_modelAccessor;
}

//------------------------------------------------------------------------------

model_includes::Model & ReporterFixture::getModel()
{
	if( !m_model )
	{
		auto model = getModelAccessor().createEmptyModel();
		INTERNAL_CHECK_WARRING( model );
		m_model.swap( model );
	}
	return *m_model;
}

//------------------------------------------------------------------------------

}
