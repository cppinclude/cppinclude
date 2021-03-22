#include "application/test/fixtures/app_test_configuration_file_fixture.hpp"

#include "application/tools/app_configuration_file.hpp"

#include "exception/ih/exc_internal_error.hpp"

#include "json/api/json_object.hpp"
#include "json/ih/json_accessor_impl.hpp"

#include "reporter/api/enums/rp_reporter_kind.hpp"
#include "reporter/tools/rp_reporter_kind_functins.hpp"

#include <optional>
#include <sstream>
#include <string>

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
	INTERNAL_CHECK_ERROR( jsonPtr != nullptr );
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

ConfigurationfileFixture::PathOpt
ConfigurationfileFixture::getCompileCommands() const
{
	return getConfigurationFile().getCompileCommands();
}

//------------------------------------------------------------------------------

ConfigurationfileFixture::StringOpt ConfigurationfileFixture::getReports() const
{
	auto reportsOpt = getConfigurationFile().getReporterKinds();
	return toStirng( reportsOpt );
}

//------------------------------------------------------------------------------

ConfigurationfileFixture::IntOpt ConfigurationfileFixture::getReportLimit() const
{
	return getConfigurationFile().getReportLimit();
}

//------------------------------------------------------------------------------

ConfigurationfileFixture::IntOpt
ConfigurationfileFixture::getReportDetailsLimit() const
{
	return getConfigurationFile().getReportDetailsLimit();
}

//------------------------------------------------------------------------------

ConfigurationfileFixture::BoolOpt
ConfigurationfileFixture::getShowStdFile() const
{
	return getConfigurationFile().getShowStdFiles();
}

//------------------------------------------------------------------------------

ConfigurationfileFixture::StringOpt
ConfigurationfileFixture::toStirng( const StringsOpt & _arrayOpt )
{
	if( !_arrayOpt )
	{
		return std::nullopt;
	}

	std::string result;

	for( const std::string & str : *_arrayOpt )
	{
		if( !result.empty() )
		{
			result += ',';
		}

		result += str;
	}

	return result;
}

//------------------------------------------------------------------------------

ConfigurationfileFixture::StringOpt
ConfigurationfileFixture::toStirng( const PathsArrayOpt & _arrayOpt )
{
	return arrayToString( _arrayOpt );
}

//------------------------------------------------------------------------------

ConfigurationfileFixture::StringOpt
ConfigurationfileFixture::toStirng( const ReporterKindsOpt & _arrayOpt )
{
	return arrayToString( _arrayOpt );
}

//------------------------------------------------------------------------------

template< class _ArrayTypeOpt >
ConfigurationfileFixture::StringOpt
ConfigurationfileFixture::arrayToString( const _ArrayTypeOpt & _arrayOpt )
{
	if( !_arrayOpt )
	{
		return std::nullopt;
	}

	Strings strings;

	for( const auto & value : *_arrayOpt )
	{
		const std::string valueStr = toString( value );
		strings.push_back( valueStr );
	}

	return toStirng( strings );
}

//------------------------------------------------------------------------------

std::string ConfigurationfileFixture::toString( const Path & _path )
{
	return _path.string();
}

//------------------------------------------------------------------------------

std::string ConfigurationfileFixture::toString( reporter::ReporterKind _kind )
{
	return reporter::reporterKindToString( _kind );
}

//------------------------------------------------------------------------------

const ConfigurationFile & ConfigurationfileFixture::getConfigurationFile() const
{
	INTERNAL_CHECK_ERROR( m_file != nullptr );
	return *m_file;
}

//------------------------------------------------------------------------------

ConfigurationFile & ConfigurationfileFixture::ensureConfigurationFile()
{
	if( !m_file )
	{
		m_file = std::make_unique< ConfigurationFile >();
	}

	return *m_file;
}

//------------------------------------------------------------------------------

json::JsonAccessor & ConfigurationfileFixture::getJsonAccessor()
{
	return m_json.ensure< json::JsonAccesorImpl >();
}

//------------------------------------------------------------------------------

}
