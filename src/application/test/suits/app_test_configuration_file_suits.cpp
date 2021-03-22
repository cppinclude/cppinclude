#include "application/test/fixtures/app_test_configuration_file_fixture.hpp"

#include "test_tools/test_macros.hpp"

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

-------------------------------------------------------------------------------*/

namespace application::test {

//------------------------------------------------------------------------------

// clazy:excludeall=non-pod-global-static
TEST_GROUP_NAME( ConfigurationfileTests, ConfigurationfileFixture )

//------------------------------------------------------------------------------

TEST_CASE( t1_empty )
{
	// Run
	loadFromJson( "{}" );

	// Check
	TEST_CHECK( !getProjectDir().has_value() );
	TEST_CHECK( !getFileExtensions().has_value() );
	TEST_CHECK( !getAnalyzeWithoutExtension().has_value() );
	TEST_CHECK( !getIncludeDirs().has_value() );
	TEST_CHECK( !getIgnoreDirs().has_value() );
	TEST_CHECK( !getIgnoreSystemIncludes().has_value() );
}

//------------------------------------------------------------------------------

TEST_CASE( t2_project_dir )
{
	// Run
	loadFromJson( R"({"project_dir":"/tmp/project"})" );

	// Check
	TEST_REQUIRE( getProjectDir().has_value() );
	BOOST_CHECK_EQUAL( getProjectDir()->string(), "/tmp/project" );
}

//------------------------------------------------------------------------------

TEST_CASE( t3_file_extenstions )
{
	// Run
	loadFromJson( R"({ "file_extensions" : [ "*.cpp", "*.hpp" ] })" );

	// Check
	auto valueOpt = getFileExtensions();

	TEST_REQUIRE( valueOpt.has_value() );
	BOOST_CHECK_EQUAL( *valueOpt, "*.cpp,*.hpp" );
}

//------------------------------------------------------------------------------

TEST_CASE( t4_include_dir )
{
	// Run
	loadFromJson( R"({ "include_dirs" : [ "include1", "include2" ] })" );

	// Check
	auto valueOpt = getIncludeDirs();

	TEST_REQUIRE( valueOpt.has_value() );
	BOOST_CHECK_EQUAL( *valueOpt, "include1,include2" );
}

//------------------------------------------------------------------------------

TEST_CASE( t5_ignore_dir )
{
	// Run
	loadFromJson( R"({ "ignore_dirs" : [ "ignore1", "ignore2" ] })" );

	// Check
	auto valueOpt = getIgnoreDirs();

	TEST_REQUIRE( valueOpt.has_value() );
	BOOST_CHECK_EQUAL( *valueOpt, "ignore1,ignore2" );
}

//------------------------------------------------------------------------------

TEST_CASE( t6_ignore_system_includes )
{
	// Run
	loadFromJson( R"({ "ignore_system_includes" : true })" );

	// Check
	auto valueOpt = getIgnoreSystemIncludes();

	TEST_REQUIRE( valueOpt.has_value() );
	BOOST_CHECK_EQUAL( *valueOpt, true );
}

//------------------------------------------------------------------------------

TEST_CASE( t7_ignore_files )
{
	// Run
	loadFromJson( R"({ "ignore_files" : [ "lib1_*", "lib2_*" ] })" );

	// Check
	auto valueOpt = getIgnoreFiles();

	TEST_REQUIRE( valueOpt.has_value() );
	BOOST_CHECK_EQUAL( *valueOpt, "lib1_*,lib2_*" );
}

//------------------------------------------------------------------------------

TEST_CASE( t8_analyze_without_extension )
{
	// Run
	loadFromJson( R"({ "analyze_without_extension" : true })" );

	// Check
	auto valueOpt = getAnalyzeWithoutExtension();

	TEST_REQUIRE( valueOpt.has_value() );
	BOOST_CHECK_EQUAL( *valueOpt, true );
}

//------------------------------------------------------------------------------

TEST_CASE( t9_1_report_unresolved )
{
	// Run
	loadFromJson( R"({ "report" : [ "unresolved" ] })" );

	// Check
	auto valueOpt = getReports();

	TEST_REQUIRE( valueOpt.has_value() );
	BOOST_CHECK_EQUAL( *valueOpt, "unresolved" );
}

//------------------------------------------------------------------------------

TEST_CASE( t9_2_report_most_impact )
{
	// Run
	loadFromJson( R"({ "report" : [ "most_impact" ] })" );

	// Check
	auto valueOpt = getReports();

	TEST_REQUIRE( valueOpt.has_value() );
	BOOST_CHECK_EQUAL( *valueOpt, "most_impact" );
}

//------------------------------------------------------------------------------

TEST_CASE( t9_3_report_dump )
{
	// Run
	loadFromJson( R"({ "report" : [ "dump" ] })" );

	// Check
	auto valueOpt = getReports();

	TEST_REQUIRE( valueOpt.has_value() );
	BOOST_CHECK_EQUAL( *valueOpt, "dump" );
}

//------------------------------------------------------------------------------

TEST_CASE( t9_4_several_reports )
{
	// Run
	loadFromJson( R"({ "report" : [ "unresolved" , "most_impact", "dump" ] })" );

	// Check
	auto valueOpt = getReports();

	TEST_REQUIRE( valueOpt.has_value() );
	BOOST_CHECK_EQUAL( *valueOpt, "unresolved,most_impact,dump" );
}

//------------------------------------------------------------------------------

TEST_CASE( t10_report_limit )
{
	// Run
	loadFromJson( R"({ "report_limit" : 10 })" );

	// Check
	auto valueOpt = getReportLimit();

	TEST_REQUIRE( valueOpt.has_value() );
	BOOST_CHECK_EQUAL( *valueOpt, 10 );
}

//------------------------------------------------------------------------------

TEST_CASE( t11_report_details_limit )
{
	// Run
	loadFromJson( R"({ "report_details_limit" : 10 })" );

	// Check
	auto valueOpt = getReportDetailsLimit();

	TEST_REQUIRE( valueOpt.has_value() );
	BOOST_CHECK_EQUAL( *valueOpt, 10 );
}

//------------------------------------------------------------------------------

TEST_CASE( t12_show_std_files )
{
	// Run
	loadFromJson( R"({ "show_std_files" : true })" );

	// Check
	auto valueOpt = getShowStdFile();

	TEST_REQUIRE( valueOpt.has_value() );
	BOOST_CHECK_EQUAL( *valueOpt, true );
}

//------------------------------------------------------------------------------

TEST_CASE( t13_compile_commands )
{
	// Run
	loadFromJson( R"({ "compile_commands" : "compile_commands.json" })" );

	// Check
	auto valueOpt = getCompileCommands();

	TEST_REQUIRE( valueOpt.has_value() );
	BOOST_CHECK_EQUAL( *valueOpt, stdfs::path{ "compile_commands.json" } );
}

//------------------------------------------------------------------------------

TEST_GROUP_END

//------------------------------------------------------------------------------

}
