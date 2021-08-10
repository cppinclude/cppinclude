#include "application/tools/app_configuration_file.hpp"

#include "application/resources/app_resources_configuration_file.hpp"

#include "exception/ih/exc_internal_error.hpp"

#include "reporter/tools/rp_reporter_kind_functins.hpp"

#include "json/api/json_array.hpp"
#include "json/api/json_object.hpp"
#include "json/api/json_value.hpp"

//------------------------------------------------------------------------------

namespace application
{
//------------------------------------------------------------------------------

void ConfigurationFile::loadFromJson( const json::JsonObject & _json )
{
	loadProjectDir( _json );
	loadFileExtensions( _json );
	loadAnalyzeWithoutExtension( _json );
	loadIncludeDirs( _json );

	loadIgnoreDirs( _json );
	loadIgnoreSystemIncludes( _json );
	loadIgnoreFiles( _json );

	loadCompileCommands( _json );

	loadReports( _json );
	loadReportLimit( _json );
	loadReportDetailsLimit( _json );
	loadShowStdFiles( _json );
	loadShowOnlyStdHeaders( _json );
	loadShowDetails( _json );

	loadThousandsSeparator( _json );
}

//------------------------------------------------------------------------------

ConfigurationFile::PathOpt ConfigurationFile::getProjectDir() const
{
	return m_projectDir;
}

//------------------------------------------------------------------------------

ConfigurationFile::StringsOpt ConfigurationFile::getFileExtensions() const
{
	return m_fileExtensions;
}

//------------------------------------------------------------------------------

ConfigurationFile::BoolOpt ConfigurationFile::getAnalyzeWithoutExtension() const
{
	return m_analyzeWithoutExtension;
}

//------------------------------------------------------------------------------

ConfigurationFile::PathsOpt ConfigurationFile::getIncludeDirs() const
{
	return m_includeDirs;
}

//------------------------------------------------------------------------------

ConfigurationFile::PathsOpt ConfigurationFile::getIgnoreDirs() const
{
	return m_ignoreDirs;
}

//------------------------------------------------------------------------------

ConfigurationFile::BoolOpt ConfigurationFile::getIgnoreSystemIncludes() const
{
	return m_ignoreSystemIncludes;
}

//------------------------------------------------------------------------------

ConfigurationFile::StringsOpt ConfigurationFile::getIgnoreFiles() const
{
	return m_ignoreFiles;
}

//------------------------------------------------------------------------------

ConfigurationFile::PathOpt ConfigurationFile::getCompileCommands() const
{
	return m_compileCommands;
}

//------------------------------------------------------------------------------

void ConfigurationFile::setCompileCommands( const Path & _path )
{
	m_compileCommands = _path;
}

//------------------------------------------------------------------------------

ConfigurationFile::ReporterKindsOpt ConfigurationFile::getReporterKinds() const
{
	return m_reports;
}

//------------------------------------------------------------------------------

ConfigurationFile::IntOpt ConfigurationFile::getReportLimit() const
{
	return m_reportLimit;
}

//------------------------------------------------------------------------------

ConfigurationFile::IntOpt ConfigurationFile::getReportDetailsLimit() const
{
	return m_reportDetailsLimit;
}

//------------------------------------------------------------------------------

ConfigurationFile::BoolOpt ConfigurationFile::getShowStdFiles() const
{
	return m_showStdFiles;
}

//------------------------------------------------------------------------------

ConfigurationFile::BoolOpt ConfigurationFile::getShowOnlyStdFiles() const
{
	return m_showOnlyStdHeaders;
}

//------------------------------------------------------------------------------

ConfigurationFile::BoolOpt ConfigurationFile::getShowDetails() const
{
	return m_showDetails;
}

//------------------------------------------------------------------------------

ConfigurationFile::StringOpt ConfigurationFile::getThousandsSeparator() const
{
	return m_thousandsSeparator;
}

//------------------------------------------------------------------------------

void ConfigurationFile::loadProjectDir( const json::JsonObject & _json )
{
	using namespace resources;

	loadPathOpt( _json, configurationFile::ProjectDir, m_projectDir );
}

//------------------------------------------------------------------------------

void ConfigurationFile::loadFileExtensions( const json::JsonObject & _json )
{
	using namespace resources;

	loadArrayOpt( _json, configurationFile::FileExtensions, m_fileExtensions );
}

//------------------------------------------------------------------------------

void ConfigurationFile::loadAnalyzeWithoutExtension(
	const json::JsonObject & _json )
{
	using namespace resources;

	loadBoolOpt(
		_json, configurationFile::AnalyzeWithoutExtension,
		m_analyzeWithoutExtension );
}

//------------------------------------------------------------------------------

void ConfigurationFile::loadIncludeDirs( const json::JsonObject & _json )
{
	using namespace resources;

	loadPathsOpt( _json, configurationFile::IncludeDirs, m_includeDirs );
}

//------------------------------------------------------------------------------

void ConfigurationFile::loadIgnoreDirs( const json::JsonObject & _json )
{
	using namespace resources;

	loadPathsOpt( _json, configurationFile::IgnoreDirs, m_ignoreDirs );
}

//------------------------------------------------------------------------------

void ConfigurationFile::loadIgnoreSystemIncludes(
	const json::JsonObject & _json )
{
	using namespace resources;

	loadBoolOpt(
		_json, configurationFile::IgnoreSystemIncludes,
		m_ignoreSystemIncludes );
}

//------------------------------------------------------------------------------

void ConfigurationFile::loadIgnoreFiles( const json::JsonObject & _json )
{
	using namespace resources;

	loadArrayOpt( _json, configurationFile::IgnoreFiles, m_ignoreFiles );
}

//------------------------------------------------------------------------------

void ConfigurationFile::loadCompileCommands( const json::JsonObject & _json )
{
	using namespace resources;

	loadPathOpt( _json, configurationFile::CompileCommands, m_compileCommands );
}

//------------------------------------------------------------------------------

void ConfigurationFile::loadReports( const json::JsonObject & _json )
{
	using namespace resources;

	StringsOpt stringsOpt;

	loadArrayOpt( _json, configurationFile::Report, stringsOpt );
	if( !stringsOpt )
	{
		return;
	}

	m_reports = ReporterKinds{};
	m_reports->reserve( stringsOpt->size() );
	for( const std::string & str: *stringsOpt )
	{
		const reporter::ReporterKind kind = reporter::toReporterKind( str );
		m_reports->push_back( kind );
	}
}

//------------------------------------------------------------------------------

void ConfigurationFile::loadReportLimit( const json::JsonObject & _json )
{
	using namespace resources;

	loadIntOpt( _json, configurationFile::ReportLimit, m_reportLimit );
}

//------------------------------------------------------------------------------

void ConfigurationFile::loadReportDetailsLimit( const json::JsonObject & _json )
{
	using namespace resources;

	loadIntOpt(
		_json, configurationFile::ReportDetailsLimit, m_reportDetailsLimit );
}

//------------------------------------------------------------------------------

void ConfigurationFile::loadShowStdFiles( const json::JsonObject & _json )
{
	using namespace resources;

	loadBoolOpt( _json, configurationFile::ShowStdFiles, m_showStdFiles );
}

//------------------------------------------------------------------------------

void ConfigurationFile::loadShowOnlyStdHeaders( const json::JsonObject & _json )
{
	using namespace resources;

	loadBoolOpt(
		_json, configurationFile::ShowOnlyStdHeaders, m_showOnlyStdHeaders );
}

//------------------------------------------------------------------------------

void ConfigurationFile::loadShowDetails( const json::JsonObject & _json )
{
	using namespace resources;

	loadBoolOpt( _json, configurationFile::ShowDetails, m_showDetails );
}

//------------------------------------------------------------------------------

void ConfigurationFile::loadThousandsSeparator( const json::JsonObject & _json )
{
	using namespace resources;

	loadStringOpt(
		_json, configurationFile::ThousandsSeparator, m_thousandsSeparator );
}

//------------------------------------------------------------------------------

void ConfigurationFile::loadStringOpt(
	const json::JsonObject & _json,
	std::string_view _name,
	StringOpt & _valueOpt )
{
	auto valuePtr = _json.getAttributeValue( _name );
	if( valuePtr )
	{
		const json::JsonValue & value = *valuePtr;
		const std::string valueStr = value.asString();
		if( !valueStr.empty() )
		{
			_valueOpt = valueStr;
		}
	}
}

//------------------------------------------------------------------------------

void ConfigurationFile::loadArrayOpt(
	const json::JsonObject & _json,
	std::string_view _name,
	StringsOpt & _arrayOpt )
{
	auto valuePtr = _json.getAttributeValue( _name );
	if( !valuePtr )
	{
		return;
	}

	const json::JsonValue & value = *valuePtr;
	auto arrayPtr = value.asArray();
	if( !arrayPtr )
	{
		return;
	}

	const json::JsonArray & array = *arrayPtr;

	if( array.empty() )
	{
		return;
	}

	const json::JsonArray::ArrayIndex count = array.getSize();
	if( !_arrayOpt )
	{
		_arrayOpt = Strings{};
	}

	_arrayOpt->reserve( count );
	for( json::JsonArray::ArrayIndex i = 0; i < count; ++i )
	{
		auto arrayValuePtr = array.at( i );
		INTERNAL_CHECK_WARRING( arrayValuePtr );
		if( !arrayValuePtr )
		{
			continue;
		}

		const json::JsonValue & arrayValue = *arrayValuePtr;
		const std::string arrayValueStr = arrayValue.asString();
		_arrayOpt->push_back( arrayValueStr );
	}
}

//------------------------------------------------------------------------------

void ConfigurationFile::loadPathOpt(
	const json::JsonObject & _json,
	std::string_view _name,
	PathOpt & _valueOpt )
{
	StringOpt pathStrOpt;
	loadStringOpt( _json, _name, pathStrOpt );

	_valueOpt = pathStrOpt;
}

//------------------------------------------------------------------------------

void ConfigurationFile::loadPathsOpt(
	const json::JsonObject & _json,
	std::string_view _name,
	PathsOpt & _valueOpt )
{
	StringsOpt stringsOpt;
	loadArrayOpt( _json, _name, stringsOpt );
	if( !stringsOpt )
	{
		return;
	}

	if( !_valueOpt )
	{
		_valueOpt = Paths{};
	}

	_valueOpt->reserve( stringsOpt->size() );

	for( const std::string & string: *stringsOpt )
	{
		_valueOpt->push_back( string );
	}
}

//------------------------------------------------------------------------------

void ConfigurationFile::loadBoolOpt(
	const json::JsonObject & _json,
	std::string_view _name,
	BoolOpt & _valueOpt )
{
	if( auto valuePtr = _json.getAttributeValue( _name ); valuePtr )
	{
		const json::JsonValue & value = *valuePtr;
		const bool valueBool = value.asBool();
		_valueOpt = valueBool;
	}
}

//------------------------------------------------------------------------------

void ConfigurationFile::loadIntOpt(
	const json::JsonObject & _json, std::string_view _name, IntOpt & _valueOpt )
{
	if( auto valuePtr = _json.getAttributeValue( _name ); valuePtr )
	{
		const json::JsonValue & value = *valuePtr;
		const int valueInt = value.asInt();
		_valueOpt = valueInt;
	}
}

//------------------------------------------------------------------------------

}
