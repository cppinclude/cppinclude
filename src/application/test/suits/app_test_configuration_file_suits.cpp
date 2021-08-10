#include "application/test/fixtures/app_test_configuration_file_fixture.hpp"

#include <boost/test/unit_test.hpp>

#include <optional>
#include <std_fs>
#include <string>

/*------------------------------------------------------------------------------

TEST PLAN
1. Empty
2. Project dir
3. File extensions4. Include dirs
5. Ignore dirs
6. Ignore system includes
7. Ignore files
8. Analyze without extension
9. Reports
	9.1	Unresolved
	9.2 MostImpact
	9.3 Dump
	9.4 Several reports
10. Report limit
11. Report details limit
12. Show std files
13. Compile commands
14. Show only std files
15. Show details
16. Thousands separator

-------------------------------------------------------------------------------*/

namespace application::test
{
//------------------------------------------------------------------------------

// clazy:excludeall=non-pod-global-static
// NOLINTNEXTLINE(fuchsia-statically-constructed-objects,cert-err58-cpp)
BOOST_FIXTURE_TEST_SUITE( ConfigurationfileTests, ConfigurationfileFixture )

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE( t1_empty )
{
	// Run
	loadFromJson( "{}" );

	// Check
	BOOST_CHECK( !getProjectDir().has_value() );
	BOOST_CHECK( !getFileExtensions().has_value() );
	BOOST_CHECK( !getAnalyzeWithoutExtension().has_value() );
	BOOST_CHECK( !getIncludeDirs().has_value() );
	BOOST_CHECK( !getIgnoreDirs().has_value() );
	BOOST_CHECK( !getIgnoreSystemIncludes().has_value() );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE( t2_project_dir )
{
	// Run
	loadFromJson( R"({"project_dir":"/tmp/project"})" );

	// Check
	BOOST_REQUIRE( getProjectDir().has_value() );
	BOOST_CHECK_EQUAL( getProjectDir()->string(), "/tmp/project" );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE( t3_file_extenstions )
{
	// Run
	loadFromJson( R"({ "file_extensions" : [ "*.cpp", "*.hpp" ] })" );

	// Check
	auto valueOpt = getFileExtensions();

	BOOST_REQUIRE( valueOpt.has_value() );
	BOOST_CHECK_EQUAL( *valueOpt, "*.cpp,*.hpp" );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE( t4_include_dir )
{
	// Run
	loadFromJson( R"({ "include_dirs" : [ "include1", "include2" ] })" );

	// Check
	auto valueOpt = getIncludeDirs();

	BOOST_REQUIRE( valueOpt.has_value() );
	BOOST_CHECK_EQUAL( *valueOpt, "include1,include2" );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE( t5_ignore_dir )
{
	// Run
	loadFromJson( R"({ "ignore_dirs" : [ "ignore1", "ignore2" ] })" );

	// Check
	auto valueOpt = getIgnoreDirs();

	BOOST_REQUIRE( valueOpt.has_value() );
	BOOST_CHECK_EQUAL( *valueOpt, "ignore1,ignore2" );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE( t6_ignore_system_includes )
{
	// Run
	loadFromJson( R"({ "ignore_system_includes" : true })" );

	// Check
	auto valueOpt = getIgnoreSystemIncludes();

	BOOST_REQUIRE( valueOpt.has_value() );
	BOOST_CHECK_EQUAL( *valueOpt, true );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE( t7_ignore_files )
{
	// Run
	loadFromJson( R"({ "ignore_files" : [ "lib1_*", "lib2_*" ] })" );

	// Check
	auto valueOpt = getIgnoreFiles();

	BOOST_REQUIRE( valueOpt.has_value() );
	BOOST_CHECK_EQUAL( *valueOpt, "lib1_*,lib2_*" );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE( t8_analyze_without_extension )
{
	// Run
	loadFromJson( R"({ "analyze_without_extension" : true })" );

	// Check
	auto valueOpt = getAnalyzeWithoutExtension();

	BOOST_REQUIRE( valueOpt.has_value() );
	BOOST_CHECK_EQUAL( *valueOpt, true );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE( t9_1_report_unresolved )
{
	// Run
	loadFromJson( R"({ "report" : [ "unresolved" ] })" );

	// Check
	auto valueOpt = getReports();

	BOOST_REQUIRE( valueOpt.has_value() );
	BOOST_CHECK_EQUAL( *valueOpt, "unresolved" );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE( t9_2_report_most_impact )
{
	// Run
	loadFromJson( R"({ "report" : [ "most_impact" ] })" );

	// Check
	auto valueOpt = getReports();

	BOOST_REQUIRE( valueOpt.has_value() );
	BOOST_CHECK_EQUAL( *valueOpt, "most_impact" );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE( t9_3_report_dump )
{
	// Run
	loadFromJson( R"({ "report" : [ "dump" ] })" );

	// Check
	auto valueOpt = getReports();

	BOOST_REQUIRE( valueOpt.has_value() );
	BOOST_CHECK_EQUAL( *valueOpt, "dump" );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE( t9_4_several_reports )
{
	// Run
	loadFromJson(
		R"({ "report" : [ "unresolved" , "most_impact", "dump" ] })" );

	// Check
	auto valueOpt = getReports();

	BOOST_REQUIRE( valueOpt.has_value() );
	BOOST_CHECK_EQUAL( *valueOpt, "unresolved,most_impact,dump" );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE( t10_report_limit )
{
	// Run
	loadFromJson( R"({ "report_limit" : 10 })" );

	// Check
	auto valueOpt = getReportLimit();

	BOOST_REQUIRE( valueOpt.has_value() );
	BOOST_CHECK_EQUAL( *valueOpt, 10 );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE( t11_report_details_limit )
{
	// Run
	loadFromJson( R"({ "report_details_limit" : 10 })" );

	// Check
	auto valueOpt = getReportDetailsLimit();

	BOOST_REQUIRE( valueOpt.has_value() );
	BOOST_CHECK_EQUAL( *valueOpt, 10 );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE( t12_show_std_files )
{
	// Run
	loadFromJson( R"({ "show_std_files" : true })" );

	// Check
	auto valueOpt = getShowStdFile();

	BOOST_REQUIRE( valueOpt.has_value() );
	BOOST_CHECK_EQUAL( *valueOpt, true );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE( t13_compile_commands )
{
	// Run
	loadFromJson( R"({ "compile_commands" : "compile_commands.json" })" );

	// Check
	auto valueOpt = getCompileCommands();

	BOOST_REQUIRE( valueOpt.has_value() );
	BOOST_CHECK_EQUAL( *valueOpt, stdfs::path{ "compile_commands.json" } );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE( t14_show_only_std_headers )
{
	// Run
	loadFromJson( R"({ "show_only_std_headers" : true })" );

	// Check
	auto valueOpt = getShowOnlyStdHeaders();

	BOOST_REQUIRE( valueOpt.has_value() );
	BOOST_CHECK_EQUAL( *valueOpt, true );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE( t15_show_details )
{
	// Run
	loadFromJson( R"({ "show_details" : true })" );

	// Check
	auto valueOpt = getShowDetails();

	BOOST_REQUIRE( valueOpt.has_value() );
	BOOST_CHECK_EQUAL( *valueOpt, true );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE( t16_thousands_separator )
{
	// Run
	loadFromJson( R"({ "thousands_separator" : "," })" );

	// Check
	auto valueOpt = getThousandsSeparator();

	BOOST_REQUIRE( valueOpt.has_value() );
	BOOST_CHECK_EQUAL( *valueOpt, "," );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE_END()

//------------------------------------------------------------------------------

}
