#include "application/test/fixtures/app_test_parser_wrapper_fixture.hpp"

#include "application/tools/app_parser_arg_wrapper.hpp"
#include "application/resources/app_resources_arguments.hpp"

#include "reporter/api/enums/rp_reporter_kind.hpp"

#include "exception/ih/exc_internal_error.hpp"

#include <std_fs>
#include <vector>

//------------------------------------------------------------------------------

namespace application::test {

//------------------------------------------------------------------------------

ParserWrapperFixture::ParserWrapperFixture() = default;
ParserWrapperFixture::~ParserWrapperFixture() = default;

//------------------------------------------------------------------------------

void ParserWrapperFixture::parse( const Arguments & _arguments )
{
	Arguments arguments{ _arguments };
	arguments.insert( arguments.begin(),  "./application" );

    std::vector< char * > arg;
	arg.reserve( arguments.size() );
	for( std::string & str : arguments )
        arg.push_back( str.data() );

    ParserArgWrapper & parser = getParser();
    parser.parse( static_cast< int >( arg.size() ), arg.data() );
}

//------------------------------------------------------------------------------

std::string ParserWrapperFixture::getProjectDir() const
{
    auto valueOpt = getParser().getProjectDir();
    if( valueOpt )
    {
        return valueOpt->string();
    }
    else
    {
        return "";
    }
}

//------------------------------------------------------------------------------

ParserWrapperFixture::StringsArray ParserWrapperFixture::getFileExtensions() const
{
	auto valueOpt = getParser().getFileExtensions();
    if( valueOpt )
        return *valueOpt;

    return StringsArray{};
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
	auto valueOpt = getParser().getAnalyzeWithoutExtension();
	if( valueOpt )
	{
		return *valueOpt;
	}
	return false;
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
    auto valueOpt = getParser().getIncludeDirs();
    if( valueOpt )
        return *valueOpt;

    return Paths{};
}

//------------------------------------------------------------------------------

ParserWrapperFixture::Paths ParserWrapperFixture::getDefaultIncludeDirs() const
{
    return getParser().getDefaultIncludeDirs();
}

//------------------------------------------------------------------------------

ParserWrapperFixture::Paths ParserWrapperFixture::getIgnoreDirs() const
{
    auto valueOpt = getParser().getIgnoreDirs();
    if( valueOpt )
        return *valueOpt;

    return Paths{};
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
		return toString( *valueOpt );

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
    auto valueOpt = getParser().getConfigurationFile();
    if( valueOpt )
        return *valueOpt;

    return Path{};
}

//------------------------------------------------------------------------------

ParserWrapperFixture::Path ParserWrapperFixture::getDefaultConfigurationFile() const
{
    return getParser().getDefaultConfigurationFile();
}

//------------------------------------------------------------------------------

ParserWrapperFixture::ReporterKinds ParserWrapperFixture::getReporterKinds() const
{
	auto kindsOpt = getParser().getReporterKinds();
	if( kindsOpt )
		return *kindsOpt;

	return ReporterKinds{};
}

//------------------------------------------------------------------------------

ParserWrapperFixture::ReporterKinds ParserWrapperFixture::getDefaultReporterKinds() const
{
	return getParser().getDefaultReporterKinds();
}

//------------------------------------------------------------------------------

int ParserWrapperFixture::getReportLimit() const
{
	auto numberOpt = getParser().getReportLimit();
	if( numberOpt )
		return *numberOpt;
	return -1;
}

//------------------------------------------------------------------------------

int ParserWrapperFixture::getDefaultReportLimit() const
{
	return getParser().getDefaultReportLimit();
}

//------------------------------------------------------------------------------

int ParserWrapperFixture::getReportDetailsLimit() const
{
	auto numberOpt = getParser().getReportDetailsLimit();
	if( numberOpt )
		return *numberOpt;
	return -1;
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
	return getParser().getDefaultShowStdfile();
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
            result += ',';
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
            result += ',';
    }
    return result;
}

//------------------------------------------------------------------------------

std::string ParserWrapperFixture::toString( const ReporterKinds & _array ) const
{
	StringsArray strings;
	strings.reserve( _array.size() );

	for( reporter::ReporterKind kind : _array )
	{
		strings.push_back( toString( kind ) );
	}

	return toString( strings );
}

//------------------------------------------------------------------------------

std::string ParserWrapperFixture::toString( reporter::ReporterKind _kind ) const
{
	using namespace reporter;
	using namespace resources::arguments;

	static_assert( static_cast< int >( ReporterKind::Count )  == 3 );
	switch( _kind )
	{
		case ReporterKind::Unresolved	: return report::UnresolvedReport;
		case ReporterKind::MostImpact	: return report::MostImpactReport;
		case ReporterKind::Dump			: return report::DumpReport;
		case ReporterKind::Count :
		{
			THROW_INTERNAL_ERROR
			return "";
		}
	}

	THROW_INTERNAL_ERROR
	return "";
}

//------------------------------------------------------------------------------

std::string ParserWrapperFixture::toString( const char * const _values[] ) const
{
    std::string result;
    const char * const * it = _values;
    while( *it )
    {
        if( !result.empty() )
            result += ',' ;

        const std::string str{ *it };
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
        m_parser.reset( new ParserArgWrapper );
		m_parser->init();
    }
    return *m_parser;
}

//------------------------------------------------------------------------------

}
