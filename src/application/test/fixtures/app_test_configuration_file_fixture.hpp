#pragma once

#include "application/tools/app_plugin_ptr.hpp"

#include <memory>
#include <stdfwd/optional>
#include <stdfwd/vector>
#include <std_fs>
#include <stdfwd/string_view>

//------------------------------------------------------------------------------

namespace json {
	class JsonAccessor;
}

namespace application {
	class ConfigurationFile;
}

namespace reporter {
	enum class ReporterKind;
}

//------------------------------------------------------------------------------

namespace application::test {

//------------------------------------------------------------------------------

class ConfigurationfileFixture
{
public:

	using StringOpt			= stdfwd::optional< std::string >;
	using Strings			= stdfwd::vector< std::string >;
	using StringsOpt		= std::optional< Strings >;

	using Path				= stdfs::path;
	using PathOpt			= std::optional< Path >;
	using PathsArray		= stdfwd::vector< Path >;
	using PathsArrayOpt		= std::optional< PathsArray >;

	using BoolOpt			= std::optional< bool >;
	using IntOpt			= std::optional< int >;

	using ReporterKinds		= stdfwd::vector< reporter::ReporterKind >;
	using ReporterKindsOpt	= std::optional< ReporterKinds >;

	ConfigurationfileFixture();
	~ConfigurationfileFixture();

	void loadFromJson( std::string_view _json );

	PathOpt getProjectDir() const;
	StringOpt getFileExtensions() const;
	BoolOpt getAnalyzeWithoutExtension() const;
	StringOpt getIncludeDirs() const;

	StringOpt getIgnoreDirs() const;
	BoolOpt getIgnoreSystemIncludes() const;
	StringOpt getIgnoreFiles() const;

	PathOpt getCompileCommands() const;

	StringOpt getReports() const;

	IntOpt getReportLimit() const;
	IntOpt getReportDetailsLimit() const;
	BoolOpt getShowStdFile() const;
	BoolOpt getShowOnlyStdHeaders() const;

	static StringOpt toStirng( const StringsOpt & _arrayOpt );
	static StringOpt toStirng( const PathsArrayOpt & _arrayOpt );
	static StringOpt toStirng( const ReporterKindsOpt & _arrayOpt );

	template< class _ArrayTypeOpt >
	static StringOpt arrayToString( const _ArrayTypeOpt & _arrayOpt );

	static std::string toString( reporter::ReporterKind _kind );
	static std::string toString( const Path & _path );

private:
	const ConfigurationFile & getConfigurationFile() const;
	ConfigurationFile & ensureConfigurationFile();

	json::JsonAccessor & getJsonAccessor();

private:
	std::unique_ptr< ConfigurationFile > m_file;

	PluginPtr< json::JsonAccessor > m_json;
};

//------------------------------------------------------------------------------

}
