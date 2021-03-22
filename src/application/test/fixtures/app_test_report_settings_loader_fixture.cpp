#include "application/test/fixtures/app_test_report_settings_loader_fixture.hpp"

#include "application/tools/app_configuration_file.hpp"
#include "application/tools/app_parser_arg_wrapper.hpp"
#include "application/tools/app_report_settings_loader.hpp"

#include "reporter/api/rp_settings.hpp"
#include "reporter/ih/rp_accessor_impl.hpp"
#include "reporter/tools/rp_reporter_kind_functins.hpp"

#include "exception/ih/exc_internal_error.hpp"

#include "json/api/json_object.hpp"
#include "json/ih/json_accessor_impl.hpp"

#include <set>
#include <sstream>
#include <string>

//------------------------------------------------------------------------------

namespace application::test {

//------------------------------------------------------------------------------

ReportSettingsLoaderFixture::ReportSettingsLoaderFixture() = default;
ReportSettingsLoaderFixture::~ReportSettingsLoaderFixture() = default;

//------------------------------------------------------------------------------

void ReportSettingsLoaderFixture::parserArguments( std::string_view _argument )
{
	std::vector< std::string > arguments;
	arguments.emplace_back( "./application" );
	arguments.emplace_back( _argument.data() );

	ParserArgWrapper & parserArguments = ensureParserArguments();
	parserArguments.parse( arguments );
}

//------------------------------------------------------------------------------

void ReportSettingsLoaderFixture::createConfigurationFile( std::string_view _json )
{
	std::stringstream stream;
	stream << _json;
	auto jsonPtr = ensureJsonAccessor().createJson( stream );
	INTERNAL_CHECK_ERROR( jsonPtr );
	const json::JsonObject & json = *jsonPtr;
	ensureConfigurationFile().loadFromJson( json );
}

//------------------------------------------------------------------------------

void ReportSettingsLoaderFixture::load()
{
	ParserArgWrapper & arguments = ensureParserArguments();
	ConfigurationFile * configurationFile = getConfigurationFile();

	ReportSettingsLoader & loader = ensureLoader();

	m_settings = loader.load( arguments, configurationFile );
	m_reportKinds = loader.loadReports( arguments, configurationFile );
}

//------------------------------------------------------------------------------

ReportSettingsLoaderFixture::CountType
ReportSettingsLoaderFixture::getMaxFilesCount() const
{
	return getSettings().getMaxFilesCount();
}

//------------------------------------------------------------------------------

ReportSettingsLoaderFixture::CountType
ReportSettingsLoaderFixture::getDefaultMaxFilesCount() const
{
	return getArguments().getDefaultReportLimit();
}

//------------------------------------------------------------------------------

ReportSettingsLoaderFixture::CountType
ReportSettingsLoaderFixture::getMaxDetailsCount() const
{
	return getSettings().getMaxDetailsCount();
}

//------------------------------------------------------------------------------

ReportSettingsLoaderFixture::CountType
ReportSettingsLoaderFixture::getDefaultMaxDetailsCount() const
{
	return getArguments().getDefaultReportDetailsLimit();
}

//------------------------------------------------------------------------------

bool ReportSettingsLoaderFixture::getShowStdFiles() const
{
	return getSettings().getShowStdFiles();
}

//------------------------------------------------------------------------------

bool ReportSettingsLoaderFixture::getDefaultShowStdFiles() const
{
	return getArguments().getDefaultShowStdfile();
}

//------------------------------------------------------------------------------

std::string ReportSettingsLoaderFixture::getReports() const
{
	return toString( m_reportKinds );
}

//------------------------------------------------------------------------------

std::string ReportSettingsLoaderFixture::getDefaultReports() const
{
	return toString( getArguments().getDefaultReporterKinds() );
}

//------------------------------------------------------------------------------

ConfigurationFile * ReportSettingsLoaderFixture::getConfigurationFile() const
{
	return m_configurationFile.get();
}

//------------------------------------------------------------------------------

ConfigurationFile & ReportSettingsLoaderFixture::ensureConfigurationFile()
{
	if( !m_configurationFile )
	{
		m_configurationFile = std::make_unique< ConfigurationFile >();
	}

	return *m_configurationFile;
}

//------------------------------------------------------------------------------

reporter::Settings & ReportSettingsLoaderFixture::getSettings() const
{
	INTERNAL_CHECK_ERROR( m_settings );
	return *m_settings;
}

//------------------------------------------------------------------------------

ParserArgWrapper & ReportSettingsLoaderFixture::getArguments() const
{
	INTERNAL_CHECK_ERROR( m_arguments );
	return *m_arguments;
}

//------------------------------------------------------------------------------

ReportSettingsLoader & ReportSettingsLoaderFixture::ensureLoader()
{
	if( !m_loader )
	{
		m_loader = std::make_unique<ReportSettingsLoader>( ensureReportFactory() );
	}

	return *m_loader;
}

//------------------------------------------------------------------------------

json::JsonAccessor & ReportSettingsLoaderFixture::ensureJsonAccessor()
{
	if( !m_json )
	{
		m_json = std::make_unique< json::JsonAccesorImpl >();
	}

	return *m_json;
}

//------------------------------------------------------------------------------

ParserArgWrapper & ReportSettingsLoaderFixture::ensureParserArguments()
{
	if( !m_arguments )
	{
		m_arguments = std::make_unique< ParserArgWrapper >();
		m_arguments->init();
	}

	return *m_arguments;
}

//------------------------------------------------------------------------------

reporter::Factory & ReportSettingsLoaderFixture::ensureReportFactory()
{
	return ensureReporterAccessor().getReporterFactory();
}

//------------------------------------------------------------------------------

reporter::ReporterAccessor & ReportSettingsLoaderFixture::ensureReporterAccessor()
{
	if( !m_reporterAccessor )
	{
		m_reporterAccessor = std::make_unique< reporter::ReporterAccessorImpl >( );
	}

	return *m_reporterAccessor;
}

//------------------------------------------------------------------------------

std::string ReportSettingsLoaderFixture::toString(
	const ReporterKinds & _reports
)
{
	std::set< std::string > names;
	for( reporter::ReporterKind kind : _reports )
	{
		const std::string name = reporter::reporterKindToString( kind );
		names.insert( name );
	}

	std::string result;
	for( const std::string & name : names )
	{
		if( !result.empty() )
		{
			result += ',';
		}

		result += name;
	}
	return result;
}

//------------------------------------------------------------------------------

}
