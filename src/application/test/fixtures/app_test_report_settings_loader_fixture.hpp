#pragma once

#include "reporter/api/enums/rp_reporter_kind.hpp"

#include <stdfwd/string_view>

#include <memory>
#include <vector>
#include <string>

//------------------------------------------------------------------------------

namespace reporter {
	class Settings;
	class Factory;
	class ReporterAccessor;
}

namespace json {
	class JsonAccessor;
}

namespace application {
	class ReportSettingsLoader;
	class ParserArgWrapper;
	class ConfigurationFile;
}

//------------------------------------------------------------------------------

namespace application::test {

//------------------------------------------------------------------------------

class ReportSettingsLoaderFixture
{
public:
	using CountType = std::size_t;

	ReportSettingsLoaderFixture();
	~ReportSettingsLoaderFixture();

	void parserArguments( std::string_view _argument );
	void createConfigurationFile( std::string_view _json );

	void load();

	CountType getMaxFilesCount() const;
	CountType getDefaultMaxFilesCount() const;

	CountType getMaxDetailsCount() const;
	CountType getDefaultMaxDetailsCount() const;

	bool getShowStdFiles() const;
	bool getDefaultShowStdFiles() const;

	bool getShowOnlyStdHeaders() const;
	bool getDefaultShowOnlyStdHeaders() const;

	std::string getReports() const;
	std::string getDefaultReports() const;

private:
	using ReporterKinds = std::vector< reporter::ReporterKind >;

	ConfigurationFile * getConfigurationFile() const;
	ConfigurationFile & ensureConfigurationFile();

	reporter::Settings & getSettings() const;
	reporter::Factory & ensureReportFactory();
	reporter::ReporterAccessor & ensureReporterAccessor();

	ParserArgWrapper & getArguments() const;
	ParserArgWrapper & ensureParserArguments();

	ReportSettingsLoader & ensureLoader();

	json::JsonAccessor & ensureJsonAccessor();

	static std::string toString( const ReporterKinds & _reports );

private:
	std::unique_ptr< reporter::ReporterAccessor > m_reporterAccessor;
	std::unique_ptr< ReportSettingsLoader > m_loader;
	std::unique_ptr< ParserArgWrapper > m_arguments;

	std::unique_ptr< json::JsonAccessor > m_json;

	std::unique_ptr< reporter::Settings > m_settings;
	std::unique_ptr< ConfigurationFile > m_configurationFile;

	ReporterKinds m_reportKinds;
};

//------------------------------------------------------------------------------

}
