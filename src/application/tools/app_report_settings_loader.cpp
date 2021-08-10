#include "application/tools/app_report_settings_loader.hpp"

#include "application/exceptions/app_incorrect_thousands_separator_impl.hpp"
#include "application/tools/app_configuration_file.hpp"
#include "application/tools/app_parser_arg_wrapper.hpp"

#include "reporter/api/rp_factory.hpp"
#include "reporter/api/rp_settings.hpp"

#include "exception/ih/exc_internal_error.hpp"

#include <memory>
#include <vector>

//------------------------------------------------------------------------------

namespace application
{
//------------------------------------------------------------------------------

ReportSettingsLoader::ReportSettingsLoader(
	reporter::Factory & _reporterFactory )
	: m_reporterFactory{ _reporterFactory }
{
}

//------------------------------------------------------------------------------

ReportSettingsLoader::SettingsPtr ReportSettingsLoader::load(
	const ParserArgWrapper & _arguments,
	const ConfigurationFile * _configurationFile )
{
	auto settingsPtr = createSettings();
	INTERNAL_CHECK_WARRING( settingsPtr );
	if( !settingsPtr )
	{
		return nullptr;
	}

	reporter::Settings & settings = *settingsPtr;

	const auto maxFiles = loadMaxFilesCount( _arguments, _configurationFile );
	const auto maxDetails =
		loadMaxDetailsCount( _arguments, _configurationFile );
	const bool showStdFiles =
		loadShowStdFiles( _arguments, _configurationFile );
	const bool showOnlyStdFiles =
		loadShowOnlyStdHeaders( _arguments, _configurationFile );
	const bool showDetails = loadShowDetails( _arguments, _configurationFile );
	const char separator =
		loadThousandsSeparator( _arguments, _configurationFile );

	settings.setMaxFilesCount( maxFiles );
	settings.setMaxDetailsCount( maxDetails );
	settings.setThousandsSeparator( separator );
	settings.setShowStdFiles( showStdFiles );
	settings.setShowOnlyStdHeaders( showOnlyStdFiles );
	settings.setShowDetails( showDetails );
	if( showOnlyStdFiles )
		settings.setShowStdFiles( true );

	return settingsPtr;
}

//------------------------------------------------------------------------------

ReportSettingsLoader::ReporterKinds ReportSettingsLoader::loadReports(
	const ParserArgWrapper & _arguments,
	const ConfigurationFile * _configurationFile )
{
	if( auto reportsOpt = _arguments.getReporterKinds(); reportsOpt )
	{
		return *reportsOpt;
	}

	if( _configurationFile != nullptr )
	{
		if( auto kindOpt = _configurationFile->getReporterKinds(); kindOpt )
		{
			return *kindOpt;
		}
	}

	return _arguments.getDefaultReporterKinds();
}

//------------------------------------------------------------------------------

ReportSettingsLoader::SettingsPtr ReportSettingsLoader::createSettings()
{
	return m_reporterFactory.createSettings();
}

//------------------------------------------------------------------------------

ReportSettingsLoader::CountType ReportSettingsLoader::loadMaxFilesCount(
	const ParserArgWrapper & _arguments,
	const ConfigurationFile * _configurationFile )
{
	return getValue(
		_arguments, &ParserArgWrapper::getReportLimit,
		&ParserArgWrapper::getDefaultReportLimit, _configurationFile,
		&ConfigurationFile::getReportLimit );
}

//------------------------------------------------------------------------------

ReportSettingsLoader::CountType ReportSettingsLoader::loadMaxDetailsCount(
	const ParserArgWrapper & _arguments,
	const ConfigurationFile * _configurationFile )
{
	return getValue(
		_arguments, &ParserArgWrapper::getReportDetailsLimit,
		&ParserArgWrapper::getDefaultReportDetailsLimit, _configurationFile,
		&ConfigurationFile::getReportDetailsLimit );
}

//------------------------------------------------------------------------------

bool ReportSettingsLoader::loadShowStdFiles(
	const ParserArgWrapper & _arguments,
	const ConfigurationFile * _configurationFile )
{
	return getValue(
		_arguments, &ParserArgWrapper::getShowStdFile,
		&ParserArgWrapper::getDefaultShowStdFile, _configurationFile,
		&ConfigurationFile::getShowStdFiles );
}

//------------------------------------------------------------------------------

bool ReportSettingsLoader::loadShowOnlyStdHeaders(
	const ParserArgWrapper & _arguments,
	const ConfigurationFile * _configurationFile )
{
	return getValue(
		_arguments, &ParserArgWrapper::getShowOnlyStdHeaders,
		&ParserArgWrapper::getDefaultShowOnlyStdHeaders, _configurationFile,
		&ConfigurationFile::getShowOnlyStdFiles );
}

//------------------------------------------------------------------------------

bool ReportSettingsLoader::loadShowDetails(
	const ParserArgWrapper & _arguments,
	const ConfigurationFile * _configurationFile )
{
	return getValue(
		_arguments, &ParserArgWrapper::getShowDetails,
		&ParserArgWrapper::getDefaultShowDetails, _configurationFile,
		&ConfigurationFile::getShowDetails );
}

//------------------------------------------------------------------------------

char ReportSettingsLoader::loadThousandsSeparator(
	const ParserArgWrapper & _arguments,
	const ConfigurationFile * _configurationFile )
{
	std::string result = getValue(
		_arguments, &ParserArgWrapper::getThousandsSeparator,
		&ParserArgWrapper::getDefaultThousandsSeparator, _configurationFile,
		&ConfigurationFile::getThousandsSeparator );

	checkThousandsSeparator( result );

	return result.at( 0 );
}

//------------------------------------------------------------------------------

template< typename _ValueType >
_ValueType ReportSettingsLoader::getValue(
	const ParserArgWrapper & _arguments,
	std::optional< _ValueType > ( ParserArgWrapper::*_getValueFromArg )() const,
	_ValueType ( ParserArgWrapper::*_getDefaultValueFromArg )() const,
	const ConfigurationFile * _configurationFile,
	std::optional< _ValueType > ( ConfigurationFile::*_getValueFromFile )()
		const )
{
	if( auto valueFromArgOpt = ( _arguments.*_getValueFromArg )();
		valueFromArgOpt )
	{
		return *valueFromArgOpt;
	}

	if( _configurationFile )
	{
		auto valueFromFileOpt = ( *_configurationFile.*_getValueFromFile )();
		if( valueFromFileOpt )
		{
			return *valueFromFileOpt;
		}
	}

	return ( _arguments.*_getDefaultValueFromArg )();
}

//------------------------------------------------------------------------------

void ReportSettingsLoader::checkThousandsSeparator(
	const std::string & _separator )
{
	if( _separator.size() != 1 )
		throw IncorrectThousandsSeparatorImpl( _separator );
}

//------------------------------------------------------------------------------

}
