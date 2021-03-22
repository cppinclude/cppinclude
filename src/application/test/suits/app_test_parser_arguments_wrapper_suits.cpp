#include "application/test/fixtures/app_test_parser_wrapper_fixture.hpp"

#include "application/exceptions/app_exceptions.hpp"
#include "application/resources/app_resources_arguments.hpp"

#include "reporter/api/enums/rp_reporter_kind.hpp"
#include "reporter/exceptions/rp_exceptions.hpp"

#include "test_tools/test_macros.hpp"

#include <optional>
#include <std_fs>

/*------------------------------------------------------------------------------

TEST PLAN

1. Project dir
	1.1 From arguments
	1.2 Default
	1.3 From argument without full name
2. File extensions
	2.1 From arguments
	2.2 Default
3. Include dirs
	3.1 From arguments
	3.2 Default
4. Ignore dirs
	4.1 From arguments
	4.2 Default
5. Configuration file
	5.1 From arguments
	5.2 Default
6. Help
	6.1 From arguments
7. Verbose
8. Report
	8.1 From arguments
	8.2 Default
	8.3 Incorrect name
9. Report limit
	9.1 From arguments
	9.2 Default
10. Ignore system includes
	10.1 From arguments
	10.2 Default
11. Ignore files
	11.1 From arguments
	11.2 Default
12. Version
13. Report details limit
	13.1 From arguments
	13.2 Default
14. Analyze without extension
	14.1 From arguments
	14.2 Default
15. Show std files
	15.1 From arguments
	15.2 Default
16. Compile commands
	16.1 From arguments
	16.2 Default

-------------------------------------------------------------------------------*/

namespace application::test {

//------------------------------------------------------------------------------

// clazy:excludeall=non-pod-global-static
TEST_GROUP_NAME( ParserWrapperTets, ParserWrapperFixture )

//------------------------------------------------------------------------------

TEST_CASE( t1_1_project_dir_from_argument )
{
	// Run
	parse( "--project_dir=test_value" );

	// Check
	BOOST_CHECK_EQUAL( "test_value", getProjectDir() );
}

//------------------------------------------------------------------------------

TEST_CASE( t1_2_project_dir_default )
{
	// Run
	parse( "" );

	// Check
	BOOST_CHECK_EQUAL(
		resources::arguments::projectDir::DefaultValue,
		getDefaultProjectDir() );
}

//------------------------------------------------------------------------------

TEST_CASE( t1_3_project_dir_from_argument_without_full_name )
{
	// Init
	parse( "project_dir" );

	// Check
	BOOST_CHECK_EQUAL( "project_dir", getProjectDir() );
}

//------------------------------------------------------------------------------

TEST_CASE( t2_1_file_extensions_from_argument )
{
	// Run
	parse( "--file_extensions=*.cpp,*.hpp" );

	// Check
	auto fileExtensions = getFileExtensions();
	BOOST_REQUIRE_EQUAL( fileExtensions.size(), 2 );
	BOOST_CHECK_EQUAL( fileExtensions.at( 0 ), "*.cpp" );
	BOOST_CHECK_EQUAL( fileExtensions.at( 1 ), "*.hpp" );
}

//------------------------------------------------------------------------------

TEST_CASE( t2_2_file_extensions_default )
{
	// Run
	parse( "" );

	// Check
	BOOST_CHECK_EQUAL(
		toString( resources::arguments::fileExtensions::DefaultValue ),
		toString( getDefaultFileExtensions() )
	);
}

//------------------------------------------------------------------------------

TEST_CASE( t3_1_include_dirs_from_argument )
{
	// Run
	parse( "--include_dirs=/include/dir1,/include/dir2" );

	// Check
	auto includeDirs = getIncludeDirs();
	BOOST_REQUIRE_EQUAL( includeDirs.size(), 2 );
	BOOST_CHECK_EQUAL( includeDirs.at( 0 ), "/include/dir1" );
	BOOST_CHECK_EQUAL( includeDirs.at( 1 ), "/include/dir2" );
}

//------------------------------------------------------------------------------

TEST_CASE( t3_2_include_dirs_default )
{
	// Run
	parse( "" );

	// Check
	BOOST_CHECK_EQUAL(
		toString( resources::arguments::includeDirs::DefaultValue ),
		toString( getDefaultIncludeDirs() )
	);
}

//------------------------------------------------------------------------------

TEST_CASE( t4_1_ignore_dirs_from_argument )
{
	// Run
	parse( "--ignore_dirs=/include/dir1,/include/dir2" );

	// Check
	auto includeDirs = getIgnoreDirs();
	BOOST_REQUIRE_EQUAL( includeDirs.size(), 2 );
	BOOST_CHECK_EQUAL( includeDirs.at( 0 ), "/include/dir1" );
	BOOST_CHECK_EQUAL( includeDirs.at( 1 ), "/include/dir2" );
}

//------------------------------------------------------------------------------

TEST_CASE( t4_2_ignore_dirs_default )
{
	// Run
	parse( "" );

	// Check
	BOOST_CHECK_EQUAL(
		toString( resources::arguments::ignoreDirs::DefaultValue ),
		toString( getDefaultIgnoreDirs() )
	);
}

//------------------------------------------------------------------------------

TEST_CASE( t5_1_configuration_file_from_arguments )
{
	// Run
	parse( "--configuration_file=./file" );

	// Check
	BOOST_CHECK_EQUAL( getConfigurationFile().string(), "./file" );
}

//------------------------------------------------------------------------------

TEST_CASE( t5_2_configuration_file_default )
{
	// Run
	parse( "" );

	// Check
	BOOST_CHECK_EQUAL(
		resources::arguments::configurationFile::DefaultValue,
		getDefaultConfigurationFile().string()
	);
}

//------------------------------------------------------------------------------

TEST_CASE( t6_1_help_from_arguments )
{
	// Run
	parse( "--help" );

	// Check
	TEST_CHECK( isHelp() );
}

//------------------------------------------------------------------------------

TEST_CASE( t7_verbose )
{
	// Run
	parse( "--verbose" );

	// Check
	TEST_CHECK( isVerbose() );
}

//------------------------------------------------------------------------------

TEST_CASE( t8_1_report_from_argument )
{
	// Run
	parse( "--report=unresolved" );

	// Check
	BOOST_CHECK_EQUAL( "unresolved", toString( getReporterKinds() ) );
}

//------------------------------------------------------------------------------

TEST_CASE( t8_2_report_default )
{
	// Run
	parse( "" );

	// Check
	BOOST_CHECK_EQUAL(
		toString( resources::arguments::report::DefaultValue ),
		toString( getDefaultReporterKinds() )
	);
}

//------------------------------------------------------------------------------

TEST_CASE( t8_3_report_incorrect_name )
{
	// Check
	parse( "--report=incorrect_name" );

	TEST_CHECK_THROW( getReporterKinds(), reporter::IncorrectReport );
}

//------------------------------------------------------------------------------

TEST_CASE( t9_1_report_limit_from_argument )
{
	// Run
	parse( "--report_limit=42" );

	// Check
	BOOST_CHECK_EQUAL( getReportLimit(), 42 );
}

//------------------------------------------------------------------------------

TEST_CASE( t9_2_report_limit_default )
{
	// Run
	parse( "" );

	// Check
	BOOST_CHECK_EQUAL(
		getDefaultReportLimit(),
		resources::arguments::report_limit::DefaultValue
	);
}

//------------------------------------------------------------------------------

TEST_CASE( t10_1_ignore_system_includes_from_arguments )
{
	// Run
	parse( "--ignore_system_includes=true" );

	// Check
	auto valueOpt = getIgnoreSystemIncludes();
	TEST_REQUIRE( valueOpt.has_value() );
	TEST_CHECK_EQUAL( *valueOpt, true );
}

//------------------------------------------------------------------------------

TEST_CASE( t10_2_ignore_system_includes_default )
{
	// Run
	parse( "" );

	// Check
	BOOST_CHECK_EQUAL(
		getDefaultIgnoreSystemIncludes(),
		resources::arguments::ignoreSystemIncludes::DefaultValue
	);
}

//------------------------------------------------------------------------------

TEST_CASE( t11_1_ignore_files_from_argument )
{
	// Run
	parse( "--ignore_files=lib1_*,lib2_*" );

	// Check

	BOOST_CHECK_EQUAL( getIgnoreFiles(), "lib1_*,lib2_*" );
}

//------------------------------------------------------------------------------

TEST_CASE( t11_2_ignore_files_default )
{
	// Run
	parse( "" );

	// Check
	BOOST_CHECK_EQUAL(
		getDefaultIgnoreFiles(),
		toString( resources::arguments::ignoreFiles::DefaultValue )
	);
}

//------------------------------------------------------------------------------

TEST_CASE( t12_version )
{
	// Run
	parse( "--version" );

	// Check

	TEST_CHECK( isVersion() );
}

//------------------------------------------------------------------------------

TEST_CASE( t13_1_report_details_limit_from_argument )
{
	// Run
	parse( "--report_details_limit=42" );

	// Check
	BOOST_CHECK_EQUAL( getReportDetailsLimit(), 42 );
}

//------------------------------------------------------------------------------

TEST_CASE( t13_2_report_details_limit_default )
{
	// Run
	parse( "" );

	// Check
	BOOST_CHECK_EQUAL(
		getDefaultReportDetailsLimit(),
		resources::arguments::report_details_limit::DefaultValue
	);
}

//------------------------------------------------------------------------------

TEST_CASE( t14_1_analyze_without_extension_from_argument )
{
	// Run
	parse( "--analyze_without_extension=true" );

	// Check
	BOOST_CHECK_EQUAL( getAnalyzeWithoutExtension(), true );
}

//------------------------------------------------------------------------------

TEST_CASE( t14_2_analyze_without_extension_default )
{
	// Run
	parse( "" );

	// Check
	BOOST_CHECK_EQUAL(
		getDefaultAnalyzeWithoutExtension(),
		resources::arguments::analyze_without_extension::DefaultValue
	);
}

//------------------------------------------------------------------------------

TEST_CASE( t15_1_show_std_files_from_argument )
{
	// Run
	parse( "--show_std_files=true" );

	// Check
	auto valueOpt = getShowStdFile();
	TEST_REQUIRE( valueOpt.has_value() );
	TEST_CHECK_EQUAL( *valueOpt, true );
}

//------------------------------------------------------------------------------

TEST_CASE( t15_1_show_std_files_default )
{
	// Run
	parse( "" );

	// Check
	BOOST_CHECK_EQUAL(
		getDefaultShowStdFile(),
		resources::arguments::show_std_files::DefaultValue
	);
}

//------------------------------------------------------------------------------

TEST_CASE( t16_1_compile_commands_from_arguments )
{
	// Run
	parse( "--compile_commands=build/compile_commands.json" );

	// Check
	BOOST_CHECK_EQUAL(getCompileCommandsFile().string(), "build/compile_commands.json" );
}

//------------------------------------------------------------------------------

TEST_CASE( t16_2_compile_commands_default )
{
	// Run
	parse( "" );

	// Check
	BOOST_CHECK_EQUAL(
		resources::arguments::compileCommands::DefaultValue,
		getDefaultCompileCommandsFile().string()
	);
}

//------------------------------------------------------------------------------

TEST_GROUP_END

//------------------------------------------------------------------------------

}
