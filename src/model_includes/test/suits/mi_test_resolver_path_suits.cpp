#include "model_includes/test/fixtures/mi_test_model_includes_fixture.hpp"

#include <boost/test/unit_test.hpp>

#include <filesystem>
#include <optional>

/*------------------------------------------------------------------------------

TEST PLAN:

1. Current directory
2. Sub directory
3. Include path
	3.1. in include directory
	3.2. in include subdir
	3.3. out of project
4. Not Found

------------------------------------------------------------------------------*/

namespace model_includes::test {

//------------------------------------------------------------------------------

BOOST_FIXTURE_TEST_SUITE(ResolverPathTests, ModelIncludesFixture)

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(t1_current_directory)
{
	// Init
	setProjectDir( "/test_project/" );
	addFileToProject( "main.cpp" );
	addFileToProject( "include.hpp" );

	// Run
	PathOpt resolvedPath = resolvePath( "/test_project/main.cpp", "include.hpp" );

	// Check
	BOOST_REQUIRE( resolvedPath.has_value() );

	const std::filesystem::path exceptedPath{ "/test_project/include.hpp" };
	BOOST_CHECK_EQUAL( *resolvedPath, exceptedPath );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(t2_subdirectory)
{
	// Init
	setProjectDir( "/test_project/" );
	addFileToProject( "main.cpp" );
	addFileToProject( "sub_dir/include.hpp" );

	// Run
	PathOpt resolvedPath = resolvePath(
		"/test_project/main.cpp",
		"sub_dir/include.hpp"
	);

	// Check
	BOOST_REQUIRE( resolvedPath.has_value() );

	const std::filesystem::path exceptedPath{ "/test_project/sub_dir/include.hpp" };
	BOOST_CHECK_EQUAL( *resolvedPath, exceptedPath );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(t3_1_in_include_dir)
{
	// Init
	setProjectDir( "/test_project/" );
	addFileToProject( "main.cpp" );

	addIncludePath( "lib" );
	addFileToProject( "lib/include.hpp" );

	// Run
	PathOpt resolvedPath = resolvePath(
		"/test_project/main.cpp",
		"include.hpp"
	);

	// Check
	BOOST_REQUIRE( resolvedPath.has_value() );

	const std::filesystem::path exceptedPath{ "/test_project/lib/include.hpp" };
	BOOST_CHECK_EQUAL( *resolvedPath, exceptedPath );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(t3_2_in_include_subdir)
{
	// Init
	setProjectDir( "/test_project/" );
	addFileToProject( "main.cpp" );

	addIncludePath( "lib" );
	addFileToProject( "lib/sub/include.hpp" );

	// Run
	PathOpt resolvedPath = resolvePath(
		"/test_project/main.cpp",
		"sub/include.hpp"
	);

	// Check
	BOOST_REQUIRE( resolvedPath.has_value() );

	const std::filesystem::path exceptedPath{ "/test_project/lib/sub/include.hpp" };
	BOOST_CHECK_EQUAL( *resolvedPath, exceptedPath );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(t3_3_in_include_out_of_project)
{
	// Init
	setProjectDir( "/test_project/" );
	addFileToProject( "main.cpp" );

	addIncludePath( "/lib" );
	addFile( "/lib/include.hpp" );

	// Run
	PathOpt resolvedPath = resolvePath(
		"/test_project/main.cpp",
		"include.hpp"
	);

	// Check
	BOOST_REQUIRE( resolvedPath.has_value() );

	const std::filesystem::path exceptedPath{ "/lib/include.hpp" };
	BOOST_CHECK_EQUAL( *resolvedPath, exceptedPath );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(t4_not_found)
{
	// Init
	setProjectDir( "/test_project/" );
	addFileToProject( "main.cpp" );

	// Run
	PathOpt resolvedPath = resolvePath(
		"/test_project/main.cpp",
		"include.hpp"
	);

	// Check
	BOOST_REQUIRE( !resolvedPath.has_value() );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE_END()

//------------------------------------------------------------------------------

}
