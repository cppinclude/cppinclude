#include "application/test/fixtures/app_test_configuration_file_fixture.hpp"

#include "application/tools/app_configuration_file.hpp"

#include "exception/ih/exc_internal_error.hpp"

#include "json/ih/json_accessor_impl.hpp"
#include "json/api/json_object.hpp"

#include <optional>
#include <string>
#include <sstream>

//------------------------------------------------------------------------------

namespace application::test {

//------------------------------------------------------------------------------

ConfigurationfileFixture::ConfigurationfileFixture() = default;
ConfigurationfileFixture::~ConfigurationfileFixture() = default;

//------------------------------------------------------------------------------

void ConfigurationfileFixture::loadFromJson( std::string_view _json )
{
	std::stringstream stream;
	stream << _json;
	auto jsonPtr = getJsonAccessor().createJson( stream );
	INTERNAL_CHECK_ERROR( jsonPtr );
	const json::JsonObject & json = *jsonPtr;
	ensureConfigurationFile().loadFromJson( json );
}

//------------------------------------------------------------------------------

ConfigurationfileFixture::PathOpt ConfigurationfileFixture::getProjectDir() const
{
	return getConfigurationFile().getProjectDir();
}

//------------------------------------------------------------------------------

ConfigurationfileFixture::StringOpt
ConfigurationfileFixture::getFileExtensions() const
{
	return toStirng( getConfigurationFile().getFileExtensions() );
}

//------------------------------------------------------------------------------

ConfigurationfileFixture::BoolOpt
ConfigurationfileFixture::getAnalyzeWithoutExtension() const
{
	return getConfigurationFile().getAnalyzeWithoutExtension();
}

//------------------------------------------------------------------------------

ConfigurationfileFixture::StringOpt
ConfigurationfileFixture::getIncludeDirs() const
{
	return toStirng( getConfigurationFile().getIncludeDirs() );
}

//------------------------------------------------------------------------------

ConfigurationfileFixture::StringOpt
ConfigurationfileFixture::getIgnoreDirs() const
{
	return toStirng( getConfigurationFile().getIgnoreDirs() );
}

//------------------------------------------------------------------------------

ConfigurationfileFixture::BoolOpt
ConfigurationfileFixture::getIgnoreSystemIncludes() const
{
	return getConfigurationFile().getIgnoreSystemIncludes();
}

//------------------------------------------------------------------------------

ConfigurationfileFixture::StringOpt
ConfigurationfileFixture::getIgnoreFiles() const
{
	return toStirng( getConfigurationFile().getIgnoreFiles() );
}

//------------------------------------------------------------------------------

ConfigurationfileFixture::StringOpt
ConfigurationfileFixture::toStirng( const StringsOpt & _arrayOpt )
{
	if( !_arrayOpt )
		return std::nullopt;

	std::string result;

	for( const std::string & str : *_arrayOpt )
	{
		if( !result.empty() )
			result += ',';

		result += str;
	}

	return result;
}

//------------------------------------------------------------------------------

ConfigurationfileFixture::StringOpt
ConfigurationfileFixture::toStirng( const PathsArrayOpt & _arrayOpt )
{
	if( !_arrayOpt )
		return std::nullopt;

	std::string result;

	for( const Path & path : *_arrayOpt )
	{
		if( !result.empty() )
			result += ',';

		result += path.string();
	}

	return result;
}

//------------------------------------------------------------------------------

const ConfigurationFile & ConfigurationfileFixture::getConfigurationFile() const
{
	INTERNAL_CHECK_ERROR( m_file );
	return *m_file;
}

//------------------------------------------------------------------------------

ConfigurationFile & ConfigurationfileFixture::ensureConfigurationFile()
{
	if( !m_file )
		m_file.reset( new ConfigurationFile );

	return *m_file;
}

//------------------------------------------------------------------------------

json::JsonAccessor & ConfigurationfileFixture::getJsonAccessor()
{
	return m_json.ensure< json::JsonAccesorImpl >();
}

//------------------------------------------------------------------------------

}
