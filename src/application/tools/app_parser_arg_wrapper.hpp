#pragma once

#include "application/tools/app_parser_arg.hpp"
#include "application/tools/app_argument.hpp"

#include <stdfwd.hpp>

//------------------------------------------------------------------------------

namespace reporter {
	enum class ReporterKind;
}

//------------------------------------------------------------------------------

namespace application {

//------------------------------------------------------------------------------

class ParserArgWrapper
{
public:

	ParserArgWrapper();

	void parse( int _argc, char * _argv[] );
	void init();

	using Path				= stdfs::path;
	using PathOpt			= std::optional< Path >;

	using StringOpt			= std::optional< std::string >;
	using Strings			= stdfwd::vector< std::string >;
	using StringsOpt		= std::optional< Strings >;

	using Paths				= stdfwd::vector< Path >;
	using PathsOpt			= std::optional< Paths >;

	using IntOpt			= std::optional< int >;
	using BoolOpt			= std::optional< bool >;

	using ReporterKinds		= stdfwd::vector< reporter::ReporterKind >;
	using ReporterKindsOpt	= std::optional< ReporterKinds >;

	PathOpt getProjectDir() const;
	Path getDefaultProjectDir() const;

	StringsOpt getFileExtensions() const;
	Strings getDefaultFileExtensions() const;

	BoolOpt getAnalyzeWithoutExtension() const;
	bool getDefaultAnalyzeWithoutExtension() const;

	PathsOpt getIncludeDirs() const;
	Paths getDefaultIncludeDirs() const;

	BoolOpt getIgnoreSystemIncludes() const;
	bool getDefaultIgnoreSystemIncludes() const;

	PathsOpt getIgnoreDirs() const;
	Paths getDefaultIgnoreDirs() const;

	StringsOpt getIgnoreFiles() const;
	Strings getDefaultIgnoreFiles() const;

	PathOpt getConfigurationFile() const;
	Path getDefaultConfigurationFile() const;

	ReporterKindsOpt getReporterKinds() const;
	ReporterKinds getDefaultReporterKinds() const;

	IntOpt getReportLimit() const;
	int getDefaultReportLimit() const;

	IntOpt getReportDetailsLimit() const;
	int getDefaultReportDetailsLimit() const;

	BoolOpt getShowStdFile() const;
	bool getDefaultShowStdfile() const;

	bool isHelp() const;
	void showHelp( std::ostream & _stream ) const;

	bool isVerbose() const;
	bool isVersion() const;

private:

	template< class _ValueType >
	void addArgument( const Argument & _argument );
	void addArgument( const Argument & _argument );

	bool isExistArg( const Argument & _arg ) const;

	template< class _ValueType >
	_ValueType getDefaultValue( const Argument & _argument ) const;

	PathOpt		getPath(		const Argument & _arg ) const;
	StringOpt	getString(		const Argument & _arg ) const;
	IntOpt		getInt(			const Argument & _arg ) const;
	BoolOpt		getBool(		const Argument & _arg ) const;
	StringsOpt	getStrings(		const Argument & _arg ) const;
	PathsOpt	getPaths(		const Argument & _arg ) const;

	template< class _Value >
	void setArgumentValue(
		Argument & _argument,
		std::optional< _Value >( ParserArgWrapper::*_getter)( const Argument & ) const
	) ;

	static Strings toStrings( const char * const _values[] );
	static Paths toPaths( const char * const _values[] );

	static ReporterKinds toReporterKinds( const Strings & _string );
	static reporter::ReporterKind toReporterKind( std::string_view _str );

private:

	ParserArg m_parser;

	Argument m_projectDirArg;
	Argument m_fileExtensionsArg;
	Argument m_analyzeWithoutextension;
	Argument m_includeDirsArg;

	Argument m_ignoreDirsArg;
	Argument m_ignoreSystemIncludes;
	Argument m_ignoreFilesArg;

	Argument m_configurationFileArg;

	Argument m_helpArg;
	Argument m_verboseArg;
	Argument m_versionArg;

	Argument m_reportArg;
	Argument m_reportLimitArg;
	Argument m_reportDetailsLimitArg;
	Argument m_showStdFilesArg;
};

//------------------------------------------------------------------------------

}
