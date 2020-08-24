#include "application/tools/app_configuration_file.hpp"

#include "application/resources/app_resources_configuration_file.hpp"

#include "exception/ih/exc_internal_error.hpp"

#include "json/api/json_object.hpp"
#include "json/api/json_value.hpp"
#include "json/api/json_array.hpp"

//------------------------------------------------------------------------------

namespace application {

//------------------------------------------------------------------------------

void ConfigurationFile::loadFromJson( const json::JsonObject & _json )
{
	loadProjectDir( _json );
	loadFileExtentions( _json );
	loadIncludeDirs( _json );

	loadIgnoreDirs( _json );
	loadIgnoreSystemIncludes( _json );
	loadIgnoreFiles( _json );
}

//------------------------------------------------------------------------------

ConfigurationFile::PathOpt ConfigurationFile::getProjectDir() const
{
	return m_projectDir;
}

//------------------------------------------------------------------------------

ConfigurationFile::StringsOpt ConfigurationFile::getFileExtensions() const
{
	return m_fileExtentions;
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

void ConfigurationFile::loadProjectDir( const json::JsonObject & _json )
{
	using namespace resources;

	StringOpt pathStrOpt;
	loadStringValue( _json, configuration_file::ProjectDir, pathStrOpt);

	m_projectDir = pathStrOpt;
}

//------------------------------------------------------------------------------

void ConfigurationFile::loadFileExtentions( const json::JsonObject & _json )
{
	using namespace resources;

	loadArrayOpt( _json, configuration_file::FileExtensions, m_fileExtentions );
}

//------------------------------------------------------------------------------

void ConfigurationFile::loadIncludeDirs( const json::JsonObject & _json )
{
	using namespace resources;

	loadPathsOpt( _json, configuration_file::IncludeDirs, m_includeDirs );
}

//------------------------------------------------------------------------------

void ConfigurationFile::loadIgnoreDirs( const json::JsonObject & _json )
{
	using namespace resources;

	loadPathsOpt( _json, configuration_file::IgnoreDirs, m_ignoreDirs );
}

//------------------------------------------------------------------------------

void ConfigurationFile::loadIgnoreSystemIncludes(
	const json::JsonObject & _json
)
{
	using namespace resources;

	loadBoolOpt( _json,
		configuration_file::IgnoreSystemIncludes,
		m_ignoreSystemIncludes
	);
}

//------------------------------------------------------------------------------

void ConfigurationFile::loadIgnoreFiles( const json::JsonObject & _json )
{
	using namespace resources;

	loadArrayOpt( _json, configuration_file::IgnoreFiles, m_ignoreFiles );
}

//------------------------------------------------------------------------------

void ConfigurationFile::loadStringValue(
	const json::JsonObject & _json,
	std::string_view _name,
	StringOpt & _valueOpt
)
{
	auto valuePtr = _json.getAttributeValue( _name );
	if( valuePtr )
	{
		const json::JsonValue & value = *valuePtr;
		const std::string valueStr = value.asString();
		if( !valueStr.empty() )
			_valueOpt = valueStr;
	}
}

//------------------------------------------------------------------------------

void ConfigurationFile::loadArrayOpt(
	const json::JsonObject & _json,
	std::string_view _name,
	StringsOpt & _arrayOpt
)
{
	auto valuePtr = _json.getAttributeValue( _name );
	if( !valuePtr )
		return;

	const json::JsonValue & value = *valuePtr;
	auto arrayPtr = value.asArray();
	if( !arrayPtr )
		return;

	const json::JsonArray & array = *arrayPtr;

	if( array.empty() )
		return;

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
			continue;

		const json::JsonValue & arrayValue = *arrayValuePtr;
		const std::string arrayValueStr = arrayValue.asString();
		_arrayOpt->push_back( arrayValueStr );
	}
}

//------------------------------------------------------------------------------

void ConfigurationFile::loadPathsOpt(
	const json::JsonObject & _json,
	std::string_view _name,
	PathsOpt & _valueOpt
)
{
	StringsOpt stringsOpt;
	loadArrayOpt( _json, _name, stringsOpt );
	if( !stringsOpt )
		return;

	if( !_valueOpt )
		_valueOpt = Paths{};

	_valueOpt->reserve( stringsOpt->size() );

	for( const std::string & string : *stringsOpt )
		_valueOpt->push_back( string );
}

//------------------------------------------------------------------------------

void ConfigurationFile::loadBoolOpt(
	const json::JsonObject & _json,
	std::string_view _name,
	BoolOpt & _valueOpt
)
{
	if( auto valuePtr = _json.getAttributeValue( _name ); valuePtr )
	{
		const json::JsonValue & value = *valuePtr;
		const bool valueBool = value.asBool();
		_valueOpt = valueBool;
	}
}

//------------------------------------------------------------------------------

}
