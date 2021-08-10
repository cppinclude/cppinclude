#include "application/test/fixtures/app_test_parser_wrapper_fixture.hpp"

#include "application/resources/app_resources_arguments.hpp"
#include "application/tools/app_parser_arg_wrapper.hpp"

#include "reporter/api/enums/rp_reporter_kind.hpp"
#include "reporter/tools/rp_reporter_kind_functins.hpp"

#include "exception/ih/exc_internal_error.hpp"

#include <std_fs>
#include <vector>

//------------------------------------------------------------------------------

namespace application::test
{
//------------------------------------------------------------------------------

ParserWrapperFixture::ParserWrapperFixture() = default;
ParserWrapperFixture::~ParserWrapperFixture() = default;

//------------------------------------------------------------------------------

void ParserWrapperFixture::parse( std::string_view _argument )
{
	Arguments arguments;
	arguments.push_back( _argument.data() );

	parse( arguments );
}

//------------------------------------------------------------------------------

void ParserWrapperFixture::parse( const Arguments & _arguments )
{
	Arguments arguments{ _arguments };
	arguments.insert( arguments.begin(), "./application" );

	ParserArgWrapper & parser = getParser();
	parser.parse( arguments );
}

//------------------------------------------------------------------------------

std::string ParserWrapperFixture::getProjectDir() const
{
	auto valueOpt = getParser().getProjectDir();
	if( valueOpt )
	{
		return valueOpt->string();
	}

	return "";
}

//------------------------------------------------------------------------------

ParserWrapperFixture::StringsArray
ParserWrapperFixture::getFileExtensions() const
{
	return getParser().getFileExtensions().value_or( StringsArray{} );
}

//------------------------------------------------------------------------------

ParserWrapperFixture::StringsArray
ParserWrapperFixture::getDefaultFileExtensions() const
{
	return getParser().getDefaultFileExtensions();
}

//------------------------------------------------------------------------------

bool ParserWrapperFixture::getAnalyzeWithoutExtension() const
{
	return getParser().getAnalyzeWithoutExtension().value_or( false );
}

//------------------------------------------------------------------------------

bool ParserWrapperFixture::getDefaultAnalyzeWithoutExtension() const
{
	return getParser().getDefaultAnalyzeWithoutExtension();
}

//------------------------------------------------------------------------------

std::string ParserWrapperFixture::getDefaultProjectDir() const
{
	return getParser().getDefaultProjectDir().string();
}

//------------------------------------------------------------------------------

ParserWrapperFixture::Paths ParserWrapperFixture::getIncludeDirs() const
{
	return getParser().getIncludeDirs().value_or( Paths{} );
}

//------------------------------------------------------------------------------

ParserWrapperFixture::Paths ParserWrapperFixture::getDefaultIncludeDirs() const
{
	return getParser().getDefaultIncludeDirs();
}

//------------------------------------------------------------------------------

ParserWrapperFixture::Paths ParserWrapperFixture::getIgnoreDirs() const
{
	return getParser().getIgnoreDirs().value_or( Paths{} );
}

//------------------------------------------------------------------------------

ParserWrapperFixture::Paths ParserWrapperFixture::getDefaultIgnoreDirs() const
{
	return getParser().getDefaultIgnoreDirs();
}

//------------------------------------------------------------------------------

std::string ParserWrapperFixture::getIgnoreFiles() const
{
	auto valueOpt = getParser().getIgnoreFiles();
	if( valueOpt )
	{
		return toString( *valueOpt );
	}

	return std::string{};
}

//------------------------------------------------------------------------------

std::string ParserWrapperFixture::getDefaultIgnoreFiles() const
{
	return toString( getParser().getDefaultIgnoreFiles() );
}

//------------------------------------------------------------------------------

ParserWrapperFixture::BoolOpt
ParserWrapperFixture::getIgnoreSystemIncludes() const
{
	return getParser().getIgnoreSystemIncludes();
}

//------------------------------------------------------------------------------

bool ParserWrapperFixture::getDefaultIgnoreSystemIncludes() const
{
	return getParser().getDefaultIgnoreSystemIncludes();
}

//------------------------------------------------------------------------------

ParserWrapperFixture::Path ParserWrapperFixture::getConfigurationFile() const
{
	return getParser().getConfigurationFile().value_or( Path{} );
}

//------------------------------------------------------------------------------

ParserWrapperFixture::Path ParserWrapperFixture::getCompileCommandsFile() const
{
	return getParser().getCompileCommandsFile().value_or( Path{} );
}

//------------------------------------------------------------------------------

ParserWrapperFixture::Path
ParserWrapperFixture::getDefaultCompileCommandsFile() const
{
	return getParser().getDefaultCompileCommandsFile();
}

//------------------------------------------------------------------------------

ParserWrapperFixture::Path
ParserWrapperFixture::getDefaultConfigurationFile() const
{
	return getParser().getDefaultConfigurationFile();
}

//------------------------------------------------------------------------------

ParserWrapperFixture::ReporterKinds
ParserWrapperFixture::getReporterKinds() const
{
	return getParser().getReporterKinds().value_or( ReporterKinds{} );
}

//------------------------------------------------------------------------------

ParserWrapperFixture::ReporterKinds
ParserWrapperFixture::getDefaultReporterKinds() const
{
	return getParser().getDefaultReporterKinds();
}

//------------------------------------------------------------------------------

int ParserWrapperFixture::getReportLimit() const
{
	return getParser().getReportLimit().value_or( -1 );
}

//------------------------------------------------------------------------------

int ParserWrapperFixture::getDefaultReportLimit() const
{
	return getParser().getDefaultReportLimit();
}

//------------------------------------------------------------------------------

int ParserWrapperFixture::getReportDetailsLimit() const
{
	return getParser().getReportDetailsLimit().value_or( -1 );
}

//------------------------------------------------------------------------------

int ParserWrapperFixture::getDefaultReportDetailsLimit() const
{
	return getParser().getDefaultReportDetailsLimit();
}

//------------------------------------------------------------------------------

ParserWrapperFixture::BoolOpt ParserWrapperFixture::getShowStdFile() const
{
	return getParser().getShowStdFile();
}

//------------------------------------------------------------------------------

bool ParserWrapperFixture::getDefaultShowStdFile() const
{
	return getParser().getDefaultShowStdFile();
}

//------------------------------------------------------------------------------

ParserWrapperFixture::BoolOpt
ParserWrapperFixture::getShowOnlyStdHeaders() const
{
	return getParser().getShowOnlyStdHeaders();
}

//------------------------------------------------------------------------------

bool ParserWrapperFixture::getDefaultShowOnlyStdHeaders() const
{
	return getParser().getDefaultShowOnlyStdHeaders();
}

//------------------------------------------------------------------------------

ParserWrapperFixture::BoolOpt ParserWrapperFixture::getShowDetails() const
{
	return getParser().getShowDetails();
}

//------------------------------------------------------------------------------

bool ParserWrapperFixture::getDefaultShowDetails() const
{
	return getParser().getDefaultShowDetails();
}

//------------------------------------------------------------------------------

ParserWrapperFixture::StringOpt
ParserWrapperFixture::getThousandsSeparator() const
{
	return getParser().getThousandsSeparator();
}

//------------------------------------------------------------------------------

std::string ParserWrapperFixture::getDefaultThousandsSeparator() const
{
	return getParser().getDefaultThousandsSeparator();
}

//------------------------------------------------------------------------------

bool ParserWrapperFixture::isHelp() const
{
	return getParser().isHelp();
}

//------------------------------------------------------------------------------

bool ParserWrapperFixture::isVerbose() const
{
	return getParser().isVerbose();
}

//------------------------------------------------------------------------------

bool ParserWrapperFixture::isVerboseIgnore() const
{
	return getParser().isVerboseIgnore();
}

//------------------------------------------------------------------------------

bool ParserWrapperFixture::isVersion() const
{
	return getParser().isVersion();
}

//------------------------------------------------------------------------------

std::string ParserWrapperFixture::toString( const StringsArray & _array ) const
{
	std::string result;
	for( std::size_t i = 0; i < _array.size(); ++i )
	{
		const std::string str = _array[i];
		result += str;
		if( i != _array.size() - 1 )
		{
			result += ',';
		}
	}
	return result;
}

//------------------------------------------------------------------------------

std::string ParserWrapperFixture::toString( const Paths & _array ) const
{
	std::string result;
	for( std::size_t i = 0; i < _array.size(); ++i )
	{
		const std::string str = _array[i].string();
		result += str;
		if( i != _array.size() - 1 )
		{
			result += ',';
		}
	}
	return result;
}

//------------------------------------------------------------------------------

std::string ParserWrapperFixture::toString( const ReporterKinds & _array ) const
{
	StringsArray strings;
	strings.reserve( _array.size() );

	for( reporter::ReporterKind kind: _array )
	{
		strings.push_back( toString( kind ) );
	}

	return toString( strings );
}

//------------------------------------------------------------------------------

std::string ParserWrapperFixture::toString( reporter::ReporterKind _kind ) const
{
	return reporter::reporterKindToString( _kind );
}

//------------------------------------------------------------------------------

// NOLINTNEXTLINE(hicpp-avoid-c-arrays, modernize-avoid-c-arrays,
// cppcoreguidelines-avoid-c-arrays)
std::string ParserWrapperFixture::toString( const char * const _values[] ) const
{
	std::string result;
	const char * const * it = _values;
	while( *it != nullptr )
	{
		if( !result.empty() )
		{
			result += ',';
		}

		const std::string str{ *it };
		// NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
		++it;
		result += str;
	}
	return result;
}

//------------------------------------------------------------------------------

ParserArgWrapper & ParserWrapperFixture::getParser() const
{
	if( !m_parser )
	{
		m_parser = std::make_unique< ParserArgWrapper >();
		m_parser->init();
	}
	return *m_parser;
}

//------------------------------------------------------------------------------

}
