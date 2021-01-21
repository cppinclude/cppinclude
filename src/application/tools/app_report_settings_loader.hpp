#pragma once

#include <stdfwd.hpp>

//------------------------------------------------------------------------------

namespace reporter {
	class Settings;
	class Factory;
	enum class ReporterKind;
}

//------------------------------------------------------------------------------

namespace application {
	class ParserArgWrapper;
	class ConfigurationFile;

//------------------------------------------------------------------------------

class ReportSettingsLoader
{
public:

	using SettingsPtr		= stdfwd::unique_ptr< reporter::Settings >;
	using ReporterKinds		= stdfwd::vector< reporter::ReporterKind >;

	using CountType			= std::size_t;

	ReportSettingsLoader( reporter::Factory & _reporterFactory );

	SettingsPtr load(
		const ParserArgWrapper & _arguments,
		const ConfigurationFile * _configurationFile
	);

	ReporterKinds loadReports(
		const ParserArgWrapper & _arguments,
		const ConfigurationFile * _configurationFile
	);

private:

	SettingsPtr createSettings();

	static CountType loadMaxFilesCount(
		const ParserArgWrapper & _arguments,
		const ConfigurationFile * _configurationFile
	);

	static CountType loadMaxDetailsCount(
		const ParserArgWrapper & _arguments,
		const ConfigurationFile * _configurationFile
	);

	static bool loadShowStdFiles(
		const ParserArgWrapper & _arguments,
		const ConfigurationFile * _configurationFile
	);

	template< typename _ValueType >
	static _ValueType getValue(
		const ParserArgWrapper & _arguments,
		std::optional< _ValueType > (ParserArgWrapper::*_getValueFromArg)() const,
		_ValueType (ParserArgWrapper::*_getDefaultValueFromArg)() const,
		const ConfigurationFile * _configurationFile,
		std::optional< _ValueType > (ConfigurationFile::*_getValueFromFile)() const
	);

private:

	reporter::Factory & m_reporterFactory;
};

//------------------------------------------------------------------------------

}
