#pragma once

#include <std_fs>
#include <stdfwd/memory>

//------------------------------------------------------------------------------

namespace json
{
class JsonAccessor;
class JsonObject;
}

namespace fs
{
class FileSystem;
class File;
}

//------------------------------------------------------------------------------

namespace application
{
class ConfigurationFile;
class ParserArgWrapper;

//------------------------------------------------------------------------------

class ConfigurationFileLoader
{
public:
	using ConfigurationFilePtr = stdfwd::unique_ptr< ConfigurationFile >;
	using Path = stdfs::path;

	using JsonAccessor = json::JsonAccessor;
	using JsonObject = json::JsonObject;
	using JsonObjectPtr = stdfwd::unique_ptr< JsonObject >;

	ConfigurationFileLoader(
		JsonAccessor & _jsonAccessor, const fs::FileSystem & _fs );

	ConfigurationFilePtr load( const ParserArgWrapper & _arguments );
	ConfigurationFilePtr load( fs::File & _file );

private:
	JsonAccessor & m_jsonAccessor;
	const fs::FileSystem & m_fs;
};

//------------------------------------------------------------------------------

}
