#include "application/tools/app_parser_arg_wrapper.hpp"

#include "application/resources/app_resources_arguments.hpp"

#include "exception/ih/exc_internal_error.hpp"
#include "reporter/api/enums/rp_reporter_kind.hpp"
#include "tools/is_vector.hpp"

#include "reporter/tools/rp_reporter_kind_functins.hpp"

#include <optional>
#include <std_fs>
#include <string>
#include <unordered_map>
#include <vector>

//------------------------------------------------------------------------------

namespace application
{
//------------------------------------------------------------------------------

ParserArgWrapper::ParserArgWrapper()
	:	m_projectDirArg{
			resources::arguments::projectDir::FullName,
			resources::arguments::projectDir::Description,
			Path( resources::arguments::projectDir::DefaultValue ),
		}
	,	m_fileExtensionsArg{
			resources::arguments::fileExtensions::FullName,
			resources::arguments::fileExtensions::Description,
			toStrings( resources::arguments::fileExtensions::DefaultValues )
		}
	,	m_analyzeWithoutextension{
			resources::arguments::analyzeWithoutExtension::FullName,
			resources::arguments::analyzeWithoutExtension::Description,
			resources::arguments::analyzeWithoutExtension::DefaultValue
	}
	,	m_includeDirsArg{
			resources::arguments::includeDirs::FullName,
			resources::arguments::includeDirs::Description,
            toPaths( resources::arguments::includeDirs::DefaultValues )
		}
	,	m_ignoreDirsArg{
			resources::arguments::ignoreDirs::FullName,
			resources::arguments::ignoreDirs::Description,
			toPaths( resources::arguments::ignoreDirs::DefaultValues )
		}
	,	m_ignoreSystemIncludes{
			resources::arguments::ignoreSystemIncludes::FullName,
			resources::arguments::ignoreSystemIncludes::Description,
			resources::arguments::ignoreSystemIncludes::DefaultValue
		}
	,	m_ignoreFilesArg{
			resources::arguments::ignoreFiles::FullName,
			resources::arguments::ignoreFiles::Description,
			toStrings( resources::arguments::ignoreFiles::DefaultValues )
		}
	,	m_configurationFileArg{
			resources::arguments::configurationFile::FullName,
			resources::arguments::configurationFile::Description,
			Path( resources::arguments::configurationFile::DefaultValue )
		}
	,	m_compileCommandsFileArg{
			resources::arguments::compileCommands::FullName,
			resources::arguments::compileCommands::Description,
			Path( resources::arguments::compileCommands::DefaultValue )
		}
	,	m_helpArg{
			resources::arguments::help::FullName,
			resources::arguments::help::Description
		}
	,	m_verboseArg{
			resources::arguments::verbose::FullName,
			resources::arguments::verbose::Description
		}
	,	m_verboseIgnoreArg{
			resources::arguments::verboseIgnore::FullName,
			resources::arguments::verboseIgnore::Description
		}
	,	m_versionArg{
			resources::arguments::version::FullName,
			resources::arguments::version::Description
		}
	,	m_reportArg{
			resources::arguments::report::FullName,
			resources::arguments::report::Description,
			toStrings( resources::arguments::report::DefaultValue ),
		}
	,	m_reportLimitArg{
			resources::arguments::reportLimit::FullName,
			resources::arguments::reportLimit::Description,
			resources::arguments::reportLimit::DefaultValue
		}
	,	m_reportDetailsLimitArg{
			resources::arguments::reportDetailsLimit::FullName,
			resources::arguments::reportDetailsLimit::Description,
			resources::arguments::reportDetailsLimit::DefaultValue
		}
	,	m_showStdFilesArg{
			resources::arguments::showStdFiles::FullName,
			resources::arguments::showStdFiles::Description,
			resources::arguments::showStdFiles::DefaultValue
		}
	,	m_showOnlyStdHeadersArg{
			resources::arguments::showOnlyStdHeaders::FullName,
			resources::arguments::showOnlyStdHeaders::Description,
			resources::arguments::showOnlyStdHeaders::DefaultValue
	}
	,	m_showDetailsArg{
			resources::arguments::showDetails::FullName,
			resources::arguments::showDetails::Description,
			resources::arguments::showDetails::DefaultValue
	}
	,	m_thousandsSeparatorArg{
			resources::arguments::thousandsSeparator::FullName,
			resources::arguments::thousandsSeparator::Description,
			std::string( resources::arguments::thousandsSeparator::DefaultValue )
	}
{
}

//------------------------------------------------------------------------------

// NOLINTNEXTLINE(hicpp-avoid-c-arrays, modernize-avoid-c-arrays,
// cppcoreguidelines-avoid-c-arrays)
void ParserArgWrapper::parse( int _argc, char * _argv[] )
{
	m_parser.parse( _argc, _argv );

	if( _argc == 2 )
	{
		// NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
		const std::string str = _argv[1];
		if( !str.empty() && str[0] != '-' )
		{
			m_projectDirArg.setValue< Path >( Path{ str } );
			return;
		}
	}

	setArgumentValue( m_projectDirArg, &ParserArgWrapper::getPath );
	setArgumentValue( m_fileExtensionsArg, &ParserArgWrapper::getStrings );
	setArgumentValue( m_analyzeWithoutextension, &ParserArgWrapper::getBool );
	setArgumentValue( m_includeDirsArg, &ParserArgWrapper::getPaths );

	setArgumentValue( m_ignoreDirsArg, &ParserArgWrapper::getPaths );
	setArgumentValue( m_ignoreSystemIncludes, &ParserArgWrapper::getBool );
	setArgumentValue( m_ignoreFilesArg, &ParserArgWrapper::getStrings );

	setArgumentValue( m_configurationFileArg, &ParserArgWrapper::getPath );
	setArgumentValue( m_compileCommandsFileArg, &ParserArgWrapper::getPath );

	setArgumentValue( m_reportArg, &ParserArgWrapper::getStrings );
	setArgumentValue( m_reportLimitArg, &ParserArgWrapper::getInt );
	setArgumentValue( m_reportDetailsLimitArg, &ParserArgWrapper::getInt );
	setArgumentValue( m_showStdFilesArg, &ParserArgWrapper::getBool );
	setArgumentValue( m_showOnlyStdHeadersArg, &ParserArgWrapper::getBool );
	setArgumentValue( m_showDetailsArg, &ParserArgWrapper::getBool );

	setArgumentValue( m_thousandsSeparatorArg, &ParserArgWrapper::getString );
}

//------------------------------------------------------------------------------

void ParserArgWrapper::parse( const stdfwd::vector< std::string > & _arguments )
{
	std::vector< std::string > arguments{ _arguments };

	std::vector< char * > arg;
	arg.reserve( arguments.size() );

	for( std::string & str: arguments )
	{
		arg.push_back( str.data() );
	}

	parse( static_cast< int >( arg.size() ), arg.data() );
}

//------------------------------------------------------------------------------

void ParserArgWrapper::init()
{
	addArgument< Strings >( m_reportArg );

	addArgument< Path >( m_configurationFileArg );
	addArgument< Path >( m_compileCommandsFileArg );

	addArgument< Path >( m_projectDirArg );
	addArgument< Strings >( m_fileExtensionsArg );
	addArgument< bool >( m_analyzeWithoutextension );
	addArgument< Paths >( m_includeDirsArg );

	addArgument< Paths >( m_ignoreDirsArg );
	addArgument< bool >( m_ignoreSystemIncludes );
	addArgument< Strings >( m_ignoreFilesArg );

	addArgument< int >( m_reportLimitArg );
	addArgument< int >( m_reportDetailsLimitArg );
	addArgument< bool >( m_showStdFilesArg );
	addArgument< bool >( m_showOnlyStdHeadersArg );
	addArgument< bool >( m_showDetailsArg );

	addArgument< std::string >( m_thousandsSeparatorArg );

	addArgument( m_helpArg );
	addArgument( m_verboseArg );
	addArgument( m_verboseIgnoreArg );
	addArgument( m_versionArg );
}

//------------------------------------------------------------------------------

ParserArgWrapper::Path ParserArgWrapper::getDefaultProjectDir() const
{
	return getDefaultValue< Path >( m_projectDirArg );
}

//------------------------------------------------------------------------------

ParserArgWrapper::PathOpt ParserArgWrapper::getProjectDir() const
{
	return m_projectDirArg.getValue< Path >();
}

//------------------------------------------------------------------------------

ParserArgWrapper::Strings ParserArgWrapper::getDefaultFileExtensions() const
{
	return getDefaultValue< Strings >( m_fileExtensionsArg );
}

//------------------------------------------------------------------------------

ParserArgWrapper::BoolOpt ParserArgWrapper::getAnalyzeWithoutExtension() const
{
	return m_analyzeWithoutextension.getValue< bool >();
}

//------------------------------------------------------------------------------

bool ParserArgWrapper::getDefaultAnalyzeWithoutExtension() const
{
	return getDefaultValue< bool >( m_analyzeWithoutextension );
}

//------------------------------------------------------------------------------

ParserArgWrapper::StringsOpt ParserArgWrapper::getFileExtensions() const
{
	return m_fileExtensionsArg.getValue< Strings >();
}

//------------------------------------------------------------------------------

ParserArgWrapper::Paths ParserArgWrapper::getDefaultIncludeDirs() const
{
	return getDefaultValue< Paths >( m_includeDirsArg );
}

//------------------------------------------------------------------------------

ParserArgWrapper::BoolOpt ParserArgWrapper::getIgnoreSystemIncludes() const
{
	return m_ignoreSystemIncludes.getValue< bool >();
}

//------------------------------------------------------------------------------

bool ParserArgWrapper::getDefaultIgnoreSystemIncludes() const
{
	return getDefaultValue< bool >( m_ignoreSystemIncludes );
}

//------------------------------------------------------------------------------

ParserArgWrapper::PathsOpt ParserArgWrapper::getIncludeDirs() const
{
	return m_includeDirsArg.getValue< Paths >();
}

//------------------------------------------------------------------------------

ParserArgWrapper::Paths ParserArgWrapper::getDefaultIgnoreDirs() const
{
	return getDefaultValue< Paths >( m_ignoreDirsArg );
}

//------------------------------------------------------------------------------

ParserArgWrapper::StringsOpt ParserArgWrapper::getIgnoreFiles() const
{
	return m_ignoreFilesArg.getValue< Strings >();
}

//------------------------------------------------------------------------------

ParserArgWrapper::Strings ParserArgWrapper::getDefaultIgnoreFiles() const
{
	return getDefaultValue< Strings >( m_ignoreFilesArg );
}

//------------------------------------------------------------------------------

ParserArgWrapper::PathsOpt ParserArgWrapper::getIgnoreDirs() const
{
	return m_ignoreDirsArg.getValue< Paths >();
}

//------------------------------------------------------------------------------

ParserArgWrapper::ReporterKinds
ParserArgWrapper::getDefaultReporterKinds() const
{
	auto strings = getDefaultValue< Strings >( m_reportArg );
	return toReporterKinds( strings );
}

//------------------------------------------------------------------------------

ParserArgWrapper::ReporterKindsOpt ParserArgWrapper::getReporterKinds() const
{
	if( StringsOpt strings = m_reportArg.getValue< Strings >(); strings )
	{
		return toReporterKinds( *strings );
	}

	return std::nullopt;
}

//------------------------------------------------------------------------------

ParserArgWrapper::IntOpt ParserArgWrapper::getReportLimit() const
{
	return m_reportLimitArg.getValue< int >();
}

//------------------------------------------------------------------------------

int ParserArgWrapper::getDefaultReportLimit() const
{
	return getDefaultValue< int >( m_reportLimitArg );
}

//------------------------------------------------------------------------------

ParserArgWrapper::IntOpt ParserArgWrapper::getReportDetailsLimit() const
{
	return m_reportDetailsLimitArg.getValue< int >();
}

//------------------------------------------------------------------------------

int ParserArgWrapper::getDefaultReportDetailsLimit() const
{
	return getDefaultValue< int >( m_reportDetailsLimitArg );
}

//------------------------------------------------------------------------------

ParserArgWrapper::BoolOpt ParserArgWrapper::getShowStdFile() const
{
	return m_showStdFilesArg.getValue< bool >();
}

//------------------------------------------------------------------------------

bool ParserArgWrapper::getDefaultShowStdFile() const
{
	return getDefaultValue< bool >( m_showStdFilesArg );
}

//------------------------------------------------------------------------------

ParserArgWrapper::BoolOpt ParserArgWrapper::getShowOnlyStdHeaders() const
{
	return m_showOnlyStdHeadersArg.getValue< bool >();
}

//------------------------------------------------------------------------------

ParserArgWrapper::BoolOpt ParserArgWrapper::getShowDetails() const
{
	return m_showDetailsArg.getValue< bool >();
}

//------------------------------------------------------------------------------

ParserArgWrapper::StringOpt ParserArgWrapper::getThousandsSeparator() const
{
	return m_thousandsSeparatorArg.getValue< std::string >();
}

//------------------------------------------------------------------------------

std::string ParserArgWrapper::getDefaultThousandsSeparator() const
{
	return getDefaultValue< std::string >( m_thousandsSeparatorArg );
}

//------------------------------------------------------------------------------

bool ParserArgWrapper::getDefaultShowDetails() const
{
	return getDefaultValue< bool >( m_showDetailsArg );
}

//------------------------------------------------------------------------------

bool ParserArgWrapper::getDefaultShowOnlyStdHeaders() const
{
	return getDefaultValue< bool >( m_showOnlyStdHeadersArg );
}

//------------------------------------------------------------------------------

ParserArgWrapper::PathOpt ParserArgWrapper::getConfigurationFile() const
{
	return m_configurationFileArg.getValue< Path >();
}

//------------------------------------------------------------------------------

ParserArgWrapper::Path ParserArgWrapper::getDefaultConfigurationFile() const
{
	return getDefaultValue< Path >( m_configurationFileArg );
}

//------------------------------------------------------------------------------

ParserArgWrapper::PathOpt ParserArgWrapper::getCompileCommandsFile() const
{
	return m_compileCommandsFileArg.getValue< Path >();
}

//------------------------------------------------------------------------------

ParserArgWrapper::Path ParserArgWrapper::getDefaultCompileCommandsFile() const
{
	return getDefaultValue< Path >( m_compileCommandsFileArg );
}

//------------------------------------------------------------------------------

template< class _ValueType >
void ParserArgWrapper::addArgument( const Argument & _argument )
{
	m_parser.addArgument(
		_argument.getFullName(), _argument.getDescription(),
		getDefaultValue< _ValueType >( _argument ) );
}

//------------------------------------------------------------------------------

bool ParserArgWrapper::isExistArg( const Argument & _arg ) const
{
	return m_parser.isExistArgument( _arg.getFullName() );
}

//------------------------------------------------------------------------------

void ParserArgWrapper::addArgument( const Argument & _argument )
{
	m_parser.addArgument( _argument.getFullName(), _argument.getDescription() );
}

//------------------------------------------------------------------------------

template< class _ValueType >
_ValueType ParserArgWrapper::getDefaultValue( const Argument & _argument ) const
{
	auto valueOpt = _argument.getDefaultValue< _ValueType >();
	if( !valueOpt )
	{
		THROW_INTERNAL_ERROR
	}
	return *valueOpt;
}

//------------------------------------------------------------------------------

ParserArgWrapper::PathOpt
ParserArgWrapper::getPath( const Argument & _arg ) const
{
	return m_parser.getArgumentPathValue( _arg.getFullName() );
}

//------------------------------------------------------------------------------

ParserArgWrapper::StringOpt
ParserArgWrapper::getString( const Argument & _arg ) const
{
	return m_parser.getArgumentStringValue( _arg.getFullName() );
}

//------------------------------------------------------------------------------

ParserArgWrapper::StringsOpt
ParserArgWrapper::getStrings( const Argument & _arg ) const
{
	return m_parser.getArgumentStringsValue( _arg.getFullName() );
}

//------------------------------------------------------------------------------

ParserArgWrapper::IntOpt ParserArgWrapper::getInt( const Argument & _arg ) const
{
	return m_parser.getArgumentIntValue( _arg.getFullName() );
}

//------------------------------------------------------------------------------

ParserArgWrapper::BoolOpt
ParserArgWrapper::getBool( const Argument & _arg ) const
{
	return m_parser.getArgumentBoolValue( _arg.getFullName() );
}

//------------------------------------------------------------------------------

ParserArgWrapper::PathsOpt
ParserArgWrapper::getPaths( const Argument & _arg ) const
{
	return m_parser.getArgumentPathsValue( _arg.getFullName() );
}

//------------------------------------------------------------------------------

bool ParserArgWrapper::isHelp() const
{
	return isExistArg( m_helpArg );
}

//------------------------------------------------------------------------------

bool ParserArgWrapper::isVerbose() const
{
	return isExistArg( m_verboseArg );
}

//------------------------------------------------------------------------------

bool ParserArgWrapper::isVerboseIgnore() const
{
	return isExistArg( m_verboseIgnoreArg );
}

//------------------------------------------------------------------------------

bool ParserArgWrapper::isVersion() const
{
	return isExistArg( m_versionArg );
}

//------------------------------------------------------------------------------

void ParserArgWrapper::showHelp( std::ostream & _stream ) const
{
	m_parser.printHelp( _stream );
}

//------------------------------------------------------------------------------

ParserArgWrapper::Strings ParserArgWrapper::toStrings(
	// NOLINTNEXTLINE(hicpp-avoid-c-arrays, modernize-avoid-c-arrays,
	// cppcoreguidelines-avoid-c-arrays)
	const char * const _values[] )
{
	Strings array;
	const char * const * it = _values;
	while( *it != nullptr )
	{
		const std::string str{ *it };
		// NOLINTNEXTLINE(cppcoreguidelines-pro-bounds-pointer-arithmetic)
		++it;
		array.push_back( str );
	}
	return array;
}

//------------------------------------------------------------------------------
// NOLINTNEXTLINE(hicpp-avoid-c-arrays, modernize-avoid-c-arrays,
// cppcoreguidelines-avoid-c-arrays)
ParserArgWrapper::Paths
ParserArgWrapper::toPaths( const char * const _values[] )
{
	Strings strings{ toStrings( _values ) };
	return Paths{ strings.begin(), strings.end() };
}

//------------------------------------------------------------------------------

template< class _Value >
void ParserArgWrapper::setArgumentValue(
	Argument & _argument,
	std::optional< _Value > ( ParserArgWrapper::*_getter )( const Argument & )
		const )
{
	if( auto valueOpt = ( this->*_getter )( _argument ); valueOpt )
	{
		_argument.setValue< _Value >( *valueOpt );
	}
}

//------------------------------------------------------------------------------

ParserArgWrapper::ReporterKinds
ParserArgWrapper::toReporterKinds( const Strings & _strings )
{
	ReporterKinds kinds;
	kinds.reserve( _strings.size() );
	for( const std::string & str: _strings )
	{
		kinds.push_back( toReporterKind( str ) );
	}
	return kinds;
}

//------------------------------------------------------------------------------

reporter::ReporterKind ParserArgWrapper::toReporterKind( std::string_view _str )
{
	return reporter::toReporterKind( _str );
}

//------------------------------------------------------------------------------

}
