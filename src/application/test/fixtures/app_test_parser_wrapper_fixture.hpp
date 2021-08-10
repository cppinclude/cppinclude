#pragma once

#include <std_fs>
#include <stdfwd/optional>
#include <stdfwd/string_view>
#include <stdfwd/vector>

#include <memory>

//------------------------------------------------------------------------------

namespace application
{
class ParserArgWrapper;
}

//------------------------------------------------------------------------------

namespace reporter
{
enum class ReporterKind;
}

//------------------------------------------------------------------------------

namespace application::test
{
//------------------------------------------------------------------------------

class ParserWrapperFixture
{
public:
	using Arguments = stdfwd::vector< std::string >;
	using StringOpt = std::optional< std::string >;
	using StringsArray = stdfwd::vector< std::string >;
	using Path = stdfs::path;
	using Paths = stdfwd::vector< Path >;
	using ReporterKinds = stdfwd::vector< reporter::ReporterKind >;

	using BoolOpt = std::optional< bool >;

	ParserWrapperFixture();
	~ParserWrapperFixture();

	void parse( std::string_view _argument );
	void parse( const Arguments & _arguments );

	std::string getProjectDir() const;
	std::string getDefaultProjectDir() const;

	StringsArray getFileExtensions() const;
	StringsArray getDefaultFileExtensions() const;

	bool getAnalyzeWithoutExtension() const;
	bool getDefaultAnalyzeWithoutExtension() const;

	Paths getIncludeDirs() const;
	Paths getDefaultIncludeDirs() const;

	Paths getIgnoreDirs() const;
	Paths getDefaultIgnoreDirs() const;

	std::string getIgnoreFiles() const;
	std::string getDefaultIgnoreFiles() const;

	BoolOpt getIgnoreSystemIncludes() const;
	bool getDefaultIgnoreSystemIncludes() const;

	Path getConfigurationFile() const;
	Path getDefaultConfigurationFile() const;

	Path getCompileCommandsFile() const;
	Path getDefaultCompileCommandsFile() const;

	ReporterKinds getReporterKinds() const;
	ReporterKinds getDefaultReporterKinds() const;

	int getReportLimit() const;
	int getDefaultReportLimit() const;

	int getReportDetailsLimit() const;
	int getDefaultReportDetailsLimit() const;

	BoolOpt getShowStdFile() const;
	bool getDefaultShowStdFile() const;

	BoolOpt getShowOnlyStdHeaders() const;
	bool getDefaultShowOnlyStdHeaders() const;

	BoolOpt getShowDetails() const;
	bool getDefaultShowDetails() const;

	StringOpt getThousandsSeparator() const;
	std::string getDefaultThousandsSeparator() const;

	bool isHelp() const;
	bool isVerbose() const;
	bool isVerboseIgnore() const;
	bool isVersion() const;

	std::string toString( const StringsArray & _array ) const;
	std::string toString( const Paths & _array ) const;
	std::string toString( const ReporterKinds & _array ) const;
	std::string toString( reporter::ReporterKind _kind ) const;
	std::string toString( const char * const _values[] ) const;

private:
	ParserArgWrapper & getParser() const;

	mutable std::unique_ptr< ParserArgWrapper > m_parser;
};

//------------------------------------------------------------------------------

}
