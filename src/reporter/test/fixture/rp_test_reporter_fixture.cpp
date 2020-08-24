#include "reporter/test/fixture/rp_test_reporter_fixture.hpp"

#include "reporter/ih/rp_accessor_impl.hpp"

#include "reporter/api/rp_factory.hpp"
#include "reporter/api/rp_reporter.hpp"

#include "model_includes/ih/mi_accessor_impl.hpp"
#include "model_includes/api/mi_model.hpp"
#include "model_includes/api/enums/mi_file_type.hpp"

#include "tools/path_string_tools.hpp"

#include "exception/ih/exc_internal_error.hpp"

#include <string>
#include <sstream>
#include <filesystem>

//------------------------------------------------------------------------------

namespace reporter::test {

//------------------------------------------------------------------------------

ReporterFixture::ReporterFixture()
	:	m_maxFilesCount{ 0 }
	,	m_maxDetailsCount{ 0 }
{

}

//------------------------------------------------------------------------------

ReporterFixture::~ReporterFixture() = default;

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

	Model & model = getModel();

	FileType defaultType = FileType::ProjectFile;

	File & sourceFile = model.ensureFile( _sourceFile, defaultType );
	File & destinationFile = model.ensureFile( _destinationFile, defaultType );

	Model::IncludeLocationInfo location{
		_location.m_line,
		_location.m_begin,
		_location.m_end
	};

	model.createInclude( location, sourceFile, destinationFile, _status, _type );
}

//------------------------------------------------------------------------------

void ReporterFixture::setProjectDir( std::string_view _dir )
{
	getModel().setProjectDir( _dir );
}

//------------------------------------------------------------------------------

void ReporterFixture::setMaxFilesCount( int _count )
{
	m_maxFilesCount = _count;
}

//------------------------------------------------------------------------------

void ReporterFixture::setMaxDetailsCount( int _count )
{
	m_maxDetailsCount = _count;
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

std::string ReporterFixture::toPath( std::string_view _str )
{
	return tools::toPath( std::string{ _str } );
}

//------------------------------------------------------------------------------

std::string ReporterFixture::runReporter( Reporter & _reporter)
{
	std::stringstream stream;
	const model_includes::Model & model = getModel();

	_reporter.setMaxFilesCount( m_maxFilesCount );
	_reporter.setMaxDetailsCount( m_maxDetailsCount );
	_reporter.report( model, stream );

	return stream.str();
}

//------------------------------------------------------------------------------

ReporterAccessor & ReporterFixture::getAccessor()
{
	if( !m_accessor )
		m_accessor.reset( new ReporterAccessorImpl );

	return *m_accessor;
}

//------------------------------------------------------------------------------

Factory & ReporterFixture::getFactory()
{
	return getAccessor().getReporterFactory();
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
