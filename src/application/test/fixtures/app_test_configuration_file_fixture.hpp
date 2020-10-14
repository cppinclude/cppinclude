#pragma once

#include "application/tools/app_plugin_ptr.hpp"

#include <stdfwd.hpp>
#include <memory>

//------------------------------------------------------------------------------

namespace json {
	class JsonAccessor;
}

//------------------------------------------------------------------------------

namespace application {
	class ConfigurationFile;
}

//------------------------------------------------------------------------------

namespace application::test {

//------------------------------------------------------------------------------

class ConfigurationfileFixture
{
public:

	using StringOpt			= stdfwd::optional< std::string >;
	using StringsArray		= stdfwd::vector< std::string >;
	using StringsOpt		= std::optional< StringsArray >;

	using Path				= stdfs::path;
	using PathOpt			= std::optional< Path >;
	using PathsArray		= stdfwd::vector< Path >;
	using PathsArrayOpt		= std::optional< PathsArray >;

	using BoolOpt			= std::optional< bool >;

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

	static StringOpt toStirng( const StringsOpt & _arrayOpt );
	static StringOpt toStirng( const PathsArrayOpt & _arrayOpt );

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
