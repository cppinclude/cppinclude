#pragma once

#include <stdfwd.hpp>

#include <optional>
#include <std_fs>
#include <string>
#include <vector>

//------------------------------------------------------------------------------

namespace json {
	class JsonObject;
}

namespace reporter {
	enum class ReporterKind;
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

	using Path				= stdfs::path;
	using PathOpt			= std::optional< Path >;
	using Paths				= stdfwd::vector< Path >;
	using PathsOpt			= std::optional< Paths >;

	using BoolOpt			= std::optional< bool >;
	using IntOpt			= std::optional< int >;

	using ReporterKinds		= stdfwd::vector< reporter::ReporterKind >;
	using ReporterKindsOpt	= std::optional< ReporterKinds >;

	void loadFromJson( const json::JsonObject & _json );

	PathOpt getProjectDir() const;
	StringsOpt getFileExtensions() const;
	BoolOpt getAnalyzeWithoutExtension() const;
	PathsOpt getIncludeDirs() const;

	PathsOpt getIgnoreDirs() const;
	BoolOpt getIgnoreSystemIncludes() const;
	StringsOpt getIgnoreFiles() const;

	PathOpt getCompileCommands() const;
	void setCompileCommands( const Path & _path );

	ReporterKindsOpt getReporterKinds() const;

	IntOpt getReportLimit() const;
	IntOpt getReportDetailsLimit() const;
	BoolOpt getShowStdFiles() const;

private:
	void loadProjectDir( const json::JsonObject & _json );
	void loadFileExtensions( const json::JsonObject & _json );
	void loadAnalyzeWithoutExtension( const json::JsonObject & _json );
	void loadIncludeDirs( const json::JsonObject & _json );

	void loadIgnoreDirs( const json::JsonObject & _json );
	void loadIgnoreSystemIncludes( const json::JsonObject & _json );
	void loadIgnoreFiles( const json::JsonObject & _json );

	void loadCompileCommands( const json::JsonObject & _json );

	void loadReports( const json::JsonObject & _json );
	void loadReportLimit( const json::JsonObject & _json );
	void loadReportDetailsLimit( const json::JsonObject & _json );
	void loadShowStdFiles( const json::JsonObject & _json );

	void loadStringValue(
		const json::JsonObject & _json,
		std::string_view _name,
		StringOpt & _valueOpt
	);

	void loadArrayOpt(
		const json::JsonObject & _json,
		std::string_view _name,
		StringsOpt & _arrayOpt
	);

	void loadPathOpt(
		const json::JsonObject & _json,
		std::string_view _name,
		PathOpt & _valueOpt
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

	void loadIntOpt(
		const json::JsonObject & _json,
		std::string_view _name,
		IntOpt & _valueOpt
	);

private:
	PathOpt m_projectDir;
	StringsOpt m_fileExtensions;
	BoolOpt m_analyzeWithoutExtension;
	PathsOpt m_includeDirs;

	PathsOpt m_ignoreDirs;
	BoolOpt m_ignoreSystemIncludes;
	StringsOpt m_ignoreFiles;

	PathOpt m_compileCommands;

	ReporterKindsOpt m_reports;

	IntOpt m_reportLimit;
	IntOpt m_reportDetailsLimit;
	BoolOpt m_showStdFiles;
};

//------------------------------------------------------------------------------

}
