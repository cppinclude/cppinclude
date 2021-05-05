#include "model_includes/test/fixtures/mi_test_model_includes_fixture.hpp"

#include <boost/test/unit_test.hpp>

#include <optional>
#include <std_fs>

/*------------------------------------------------------------------------------

TEST PLAN:

1. Current directory
2. Sub directory
3. Include path
	3.1. in include directory
	3.2. in include subdir
	3.3. out of project
4. Not Found
5. CMake project
	5.1 one file in include directory
	5.2 several files in difference directories
	5.3 not found
	5.4 mix with general project
	5.5 header includes file from cmake include
	5.6 header includes file from project include

------------------------------------------------------------------------------*/

namespace model_includes::test
{
//------------------------------------------------------------------------------
// clazy:excludeall=non-pod-global-static
// NOLINTNEXTLINE(fuchsia-statically-constructed-objects,cert-err58-cpp)
BOOST_FIXTURE_TEST_SUITE( ResolverPathTests, ModelIncludesFixture )

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE( t1_current_directory )
{
	// Init
	setProjectDir( "/test_project/" );
	addFileToProject( "main.cpp" );
	addFileToProject( "include.hpp" );

	// Run
	PathOpt resolvedPath = resolvePath( "/test_project/main.cpp", "include.hpp" );

	// Check
	BOOST_REQUIRE( resolvedPath.has_value() );

	const Path exceptedPath{ "/test_project/include.hpp" };
	BOOST_CHECK_EQUAL( *resolvedPath, exceptedPath );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE( t2_subdirectory )
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

	const Path exceptedPath{ "/test_project/sub_dir/include.hpp" };
	BOOST_CHECK_EQUAL( *resolvedPath, exceptedPath );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE( t3_1_in_include_dir )
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

	const Path exceptedPath{ "/test_project/lib/include.hpp" };
	BOOST_CHECK_EQUAL( *resolvedPath, exceptedPath );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE( t3_2_in_include_subdir )
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

	const Path exceptedPath{ "/test_project/lib/sub/include.hpp" };
	BOOST_CHECK_EQUAL( *resolvedPath, exceptedPath );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE( t3_3_in_include_out_of_project )
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

	const Path exceptedPath{ "/lib/include.hpp" };
	BOOST_CHECK_EQUAL( *resolvedPath, exceptedPath );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE( t4_not_found )
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

BOOST_AUTO_TEST_CASE( t5_1_cmake_project_one_file_in_include_dir )
{
	// Init
	setProjectDir( "/test_project/" );
	addFileToProject( "main.cpp" );

	addIncludePathToCMake( "/test_project/main.cpp", "./lib/" );
	addFileToProject( "lib/include.hpp" );

	// Run
	PathOpt resolvedPath = resolvePath(
		"/test_project/main.cpp",
		"include.hpp"
	);

	// Check
	BOOST_REQUIRE( resolvedPath.has_value() );

	const Path exceptedPath{ "/test_project/lib/include.hpp" };
	BOOST_CHECK_EQUAL( *resolvedPath, exceptedPath );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE( t5_2_cmake_project_two_files_in_different_include_dirs )
{
	// Init
	setProjectDir( "/test_project/" );
	addFileToProject( "file1.cpp" );
	addFileToProject( "file2.cpp" );

	addIncludePathToCMake( "/test_project/file1.cpp", "./lib1/" );
	addIncludePathToCMake( "/test_project/file2.cpp", "./lib2/" );

	addFileToProject( "lib1/include.hpp" );
	addFileToProject( "lib2/include.hpp" );

	{
		// Check
		PathOpt resolvedPath = resolvePath(
			"/test_project/file1.cpp",
			"include.hpp"
		);

		BOOST_REQUIRE( resolvedPath.has_value() );

		const Path exceptedPath{ "/test_project/lib1/include.hpp" };
		BOOST_CHECK_EQUAL( *resolvedPath, exceptedPath );
	}
	{
		// Check
		PathOpt resolvedPath = resolvePath(
			"/test_project/file2.cpp",
			"include.hpp"
		);

		BOOST_REQUIRE( resolvedPath.has_value() );

		const Path exceptedPath{ "/test_project/lib2/include.hpp" };
		BOOST_CHECK_EQUAL( *resolvedPath, exceptedPath );
	}
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE( t5_3_cmake_project_not_found )
{
	// Init
	setProjectDir( "/test_project/" );
	addFileToProject( "main.cpp" );

	addIncludePathToCMake( "/test_project/main.cpp", "./lib_fake/" );
	addFileToProject( "lib/include.hpp" );

	// Run
	PathOpt resolvedPath = resolvePath(
		"/test_project/main.cpp",
		"include.hpp"
	);

	// Check
	BOOST_REQUIRE( !resolvedPath.has_value() );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE( t5_4_cmake_project_mix_with_general_project )
{
	// Init
	setProjectDir( "/test_project/" );
	addFileToProject( "main.cpp" );

	addIncludePath( "./lib1/" );
	addIncludePathToCMake( "/test_project/main.cpp", "./lib2/" );
	addFileToProject( "lib1/include1.hpp" );
	addFileToProject( "lib2/include2.hpp" );

	// Run
	PathOpt resolvedPath1 = resolvePath(
		"/test_project/main.cpp",
		"include1.hpp"
	);
	PathOpt resolvedPath2 = resolvePath(
		"/test_project/main.cpp",
		"include2.hpp"
	);

	// Check
	BOOST_REQUIRE( resolvedPath1.has_value() );
	BOOST_REQUIRE( resolvedPath2.has_value() );

	const Path exceptedPath1{ "/test_project/lib1/include1.hpp" };
	const Path exceptedPath2{ "/test_project/lib2/include2.hpp" };

	BOOST_CHECK_EQUAL( *resolvedPath1, exceptedPath1 );
	BOOST_CHECK_EQUAL( *resolvedPath2, exceptedPath2 );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE( t5_5_cmake_header_include_file_from_cmake_include )
{
	// Init
	setProjectDir( "/test_project/" );
	addFileToProject( "classA.cpp" );
	addFileToProject( "classA.hpp" );
	addFileToProject( "lib/base_class.hpp" );

	addIncludePathToCMake( "/test_project/classA.cpp", "./lib/" );

	// Run
	PathOpt resolvedPath = resolvePath(
		"/test_project/classA.hpp",
		"base_class.hpp",
		"/test_project/classA.cpp"
	);

	// Check
	BOOST_REQUIRE( resolvedPath.has_value() );

	const Path exceptedPath{ "/test_project/lib/base_class.hpp" };
	BOOST_CHECK_EQUAL( *resolvedPath, exceptedPath );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE( t5_6_cmake_header_include_file_from_project_include )
{
	// Init
	setProjectDir( "/test_project/" );
	addFileToProject( "classA.cpp" );
	addFileToProject( "classA.hpp" );
	addFileToProject( "lib/base_class.hpp" );

	addIncludePath( "./lib/" );

	addIncludePathToCMake( "/test_project/classA.cpp", "./fake_lib/" );

	// Run
	PathOpt resolvedPath = resolvePath(
		"/test_project/classA.hpp",
		"base_class.hpp",
		"/test_project/classA.cpp"
	);

	// Check
	BOOST_REQUIRE( resolvedPath.has_value() );

	const Path exceptedPath{ "/test_project/lib/base_class.hpp" };
	BOOST_CHECK_EQUAL( *resolvedPath, exceptedPath );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE_END()

//------------------------------------------------------------------------------

}
