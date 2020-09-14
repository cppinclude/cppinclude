#pragma once

#include <stdfwd.hpp>

#include <optional>
#include <filesystem>
#include <vector>
#include <string>

//------------------------------------------------------------------------------

namespace json {
	class JsonObject;
}

//------------------------------------------------------------------------------

namespace application {

//------------------------------------------------------------------------------

class ConfigurationFile
{
public:

	using StringOpt			= std::optional< std::string >;
	using Strings			= stdfwd::vector< std::string >;
	using StringsOpt		= std::optional< Strings >;

	using PathOpt			= std::optional< std::filesystem::path >;
	using Paths				= stdfwd::vector< std::filesystem::path >;
	using PathsOpt			= std::optional< Paths >;

	using BoolOpt			= std::optional< bool >;

	void loadFromJson( const json::JsonObject & _json );

	PathOpt getProjectDir() const;
	StringsOpt getFileExtensions() const;
	BoolOpt getAnalyzeWithoutExtension() const;
	PathsOpt getIncludeDirs() const;

	PathsOpt getIgnoreDirs() const;
	BoolOpt getIgnoreSystemIncludes() const;
	StringsOpt getIgnoreFiles() const;

private:

	void loadProjectDir( const json::JsonObject & _json );
	void loadFileExtensions( const json::JsonObject & _json );
	void loadAnalyzeWithoutExtension( const json::JsonObject & _json );
	void loadIncludeDirs( const json::JsonObject & _json );

	void loadIgnoreDirs( const json::JsonObject & _json );
	void loadIgnoreSystemIncludes( const json::JsonObject & _json );
	void loadIgnoreFiles( const json::JsonObject & _json );

	void loadStringValue(
		const json::JsonObject & _json,
		std::string_view _name,
		StringOpt & _valueOpt
	);

	void loadArrayOpt(
		const json::JsonObject & _json,
		std::string_view _name,
		StringsOpt & _valueOpt
	);

	void loadPathsOpt(
		const json::JsonObject & _json,
		std::string_view _name,
		PathsOpt & _valueOpt
	);

	void loadBoolOpt(
		const json::JsonObject & _json,
		std::string_view _name,
		BoolOpt & _valueOpt
	);

private:

	PathOpt m_projectDir;
	StringsOpt m_fileExtensions;
	BoolOpt m_analyzeWithoutExtension;
	PathsOpt m_includeDirs;

	PathsOpt m_ignoreDirs;
	BoolOpt m_ignoreSystemIncludes;
	StringsOpt m_ignoreFiles;
};

//------------------------------------------------------------------------------

}
