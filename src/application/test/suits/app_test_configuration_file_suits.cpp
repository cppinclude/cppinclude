#include "application/test/fixtures/app_test_configuration_file_fixture.hpp"

#include <boost/test/unit_test.hpp>

#include <optional>
#include <string>
#include <filesystem>

/*------------------------------------------------------------------------------

TEST PLAN
1. Empty
2. Project dir
3. File extensions4. Include dirs
5. Ignore dirs
6. Ignore system includes
7. Ignore files

-------------------------------------------------------------------------------*/

namespace application::test {

//------------------------------------------------------------------------------

BOOST_FIXTURE_TEST_SUITE(ConfigurationfileTests, ConfigurationfileFixture)

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(empty_1)
{

	// Run
	loadFromJson( "{}" );

	// Check
	BOOST_CHECK( !getProjectDir().has_value() );
	BOOST_CHECK( !getFileExtensions().has_value() );
	BOOST_CHECK( !getIncludeDirs().has_value() );
	BOOST_CHECK( !getIgnoreDirs().has_value() );
	BOOST_CHECK( !getIgnoreSystemIncludes().has_value() );

}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(project_dir_2)
{

	// Run
	loadFromJson( R"({"project_dir":"/tmp/project"})" );

	// Check
	BOOST_REQUIRE( getProjectDir().has_value() );
	BOOST_CHECK_EQUAL( getProjectDir()->string(), "/tmp/project" );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(file_extenstions_3)
{

	// Run
	loadFromJson( R"({ "file_extensions" : [ "*.cpp", "*.hpp" ] })" );

	// Check
	auto valueOpt = getFileExtensions();

	BOOST_REQUIRE( valueOpt.has_value() );
	BOOST_CHECK_EQUAL( *valueOpt, "*.cpp,*.hpp" );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(include_dir_4)
{

	// Run
	loadFromJson( R"({ "include_dirs" : [ "include1", "include2" ] })" );

	// Check
	auto valueOpt = getIncludeDirs();

	BOOST_REQUIRE( valueOpt.has_value() );
	BOOST_CHECK_EQUAL( *valueOpt, "include1,include2" );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(ignore_dir_5)
{

	// Run
	loadFromJson( R"({ "ignore_dirs" : [ "ignore1", "ignore2" ] })" );

	// Check
	auto valueOpt = getIgnoreDirs();

	BOOST_REQUIRE( valueOpt.has_value() );
	BOOST_CHECK_EQUAL( *valueOpt, "ignore1,ignore2" );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(ignore_system_includes_6)
{

	// Run
	loadFromJson( R"({ "ignore_system_includes" : true })" );

	// Check
	auto valueOpt = getIgnoreSystemIncludes();

	BOOST_REQUIRE( valueOpt.has_value() );
	BOOST_CHECK_EQUAL( *valueOpt, true );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(ignore_files_7)
{

	// Run
	loadFromJson( R"({ "ignore_files" : [ "lib1_*", "lib2_*" ] })" );

	// Check
	auto valueOpt = getIgnoreFiles();

	BOOST_REQUIRE( valueOpt.has_value() );
	BOOST_CHECK_EQUAL( *valueOpt, "lib1_*,lib2_*" );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE_END()

//------------------------------------------------------------------------------

}
