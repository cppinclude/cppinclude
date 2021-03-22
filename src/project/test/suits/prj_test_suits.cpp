#include "project/test/fixture/prj_test_ficture.hpp"

#include "project/api/prj_exceptions.hpp"

#include "test_tools/test_macros.hpp"

#include <std_fs>

/*------------------------------------------------------------------------------

TEST PLAN:

1. Project dir
	1.1 Absolute path
	1.2 Relative path
2. Include dirs
	2.1 Absolute paths
	2.2 Relative paths
3. Ignore dirs
	3.1 Absolute paths
	3.2 Relative paths
	3.3 Is ignore dir
4. Cpp extensions
	4.1 is cpp extension
5. File ignores
	5.1 Empty
	5.2 Library
	5.3 File name
	5.4 Sub string
	5.5 Only begin part
	5.6 Invalid regex
	5.7 Double plus

------------------------------------------------------------------------------*/

namespace project::test
{
//------------------------------------------------------------------------------
// clazy:excludeall=non-pod-global-static
TEST_GROUP_NAME( ProjectTests, ProjectFixture )

//------------------------------------------------------------------------------

TEST_CASE( t1_1_project_dir_absolute_path )
{
	// Init
	const Path projectDirFullPath = "/home/user/project/";
	setProjectDir( projectDirFullPath );

	// Run
	changeAllPathsToAbsolute();

	// Check
	BOOST_CHECK_EQUAL( getProjectDir(), projectDirFullPath );
}

//------------------------------------------------------------------------------

TEST_CASE( t1_2_project_dir_relative_path )
{
	// Init
	const Path projectRelativePath = "project/";
	const Path projectAbsolutePath = toAbsolutePath( projectRelativePath );

	setProjectDir( projectRelativePath );

	// Run
	changeAllPathsToAbsolute();

	// Check
	BOOST_CHECK_EQUAL( getProjectDir(), projectAbsolutePath );
}

//------------------------------------------------------------------------------

TEST_CASE( t2_1_include_dirs_absolute_paths )
{
	// Init
	setProjectDir( "/home/user/project/" );
	addIncludeDirs( { "/usr/lib1", "/usr/lib2" } );

	// Run
	changeAllPathsToAbsolute();

	// Check
	Paths currentPaths = getIncludeDirs();
	Paths exceptedPaths{ "/usr/lib1/", "/usr/lib2/" };

	TEST_REQUIRE_EQUAL_COLLECTIONS(
		currentPaths.begin(),
		currentPaths.end(),

		exceptedPaths.begin(),
		exceptedPaths.end()
	);

}

//------------------------------------------------------------------------------

TEST_CASE( t2_2_include_dirs_relative_paths )
{
	// Init
	setProjectDir( "/project/" );
	addIncludeDirs( { "lib1", "lib2" } );

	// Run
	changeAllPathsToAbsolute();

	// Check
	Paths currentPaths = getIncludeDirs();
	Paths exceptedPaths{ "/project/lib1/", "/project/lib2/" };

	TEST_REQUIRE_EQUAL_COLLECTIONS(
		currentPaths.begin(),
		currentPaths.end(),

		exceptedPaths.begin(),
		exceptedPaths.end()
	);
}

//------------------------------------------------------------------------------

TEST_CASE( t3_1_ignore_dirs_absolute_paths )
{
	// Init
	setProjectDir( "/home/user/project/" );
	addIgnoreDirs( { "/usr/ignore1", "/usr/ignore2" } );

	// Run
	changeAllPathsToAbsolute();

	// Check
	Paths currentPaths = getIgnoreDirs();
	Paths exceptedPaths{ "/usr/ignore1/", "/usr/ignore2/" };

	TEST_REQUIRE_EQUAL_COLLECTIONS(
		currentPaths.begin(),
		currentPaths.end(),

		exceptedPaths.begin(),
		exceptedPaths.end()
	);
}

//------------------------------------------------------------------------------

TEST_CASE( t3_2_ignore_dirs_relative_paths )
{
	// Init
	setProjectDir( "/project/" );
	addIgnoreDirs( { "ignore1", "ignore2" } );

	// Run
	changeAllPathsToAbsolute();

	// Check
	Paths currentPaths = getIgnoreDirs();
	Paths exceptedPaths{ "/project/ignore1/", "/project/ignore2/" };

	TEST_REQUIRE_EQUAL_COLLECTIONS(
		currentPaths.begin(),
		currentPaths.end(),

		exceptedPaths.begin(),
		exceptedPaths.end()
	);
}

//------------------------------------------------------------------------------

TEST_CASE( t3_3_ignore_dirs_is_ignore )
{
	// Init

	setProjectDir( "/project/" );
	addIgnoreDirs( { "ignore1", "/usr/ignore2" } );

	// Run
	changeAllPathsToAbsolute();

	// Check
	TEST_CHECK( isIgnoreDir( "/project/ignore1" ) );
	TEST_CHECK( isIgnoreDir( "/usr/ignore2" ) );
}

//------------------------------------------------------------------------------

TEST_CASE( t4_1_cpp_extension_is_cpp_extension )
{
	// Run
	addCppExtensions( { "*.cpp", "*.hpp" } );

	// Check
	TEST_CHECK( isExistsCppExtension( "*.cpp" ) );
	TEST_CHECK( isExistsCppExtension( "*.hpp" ) );
	TEST_CHECK( isExistsCppExtension( ".hpp" ) );
	TEST_CHECK( !isExistsCppExtension( ".cxx" ) );
}

//------------------------------------------------------------------------------

TEST_CASE( t5_1_ignore_files_empty )
{
	// Init
	changeAllPathsToAbsolute(); // create project

	// Check
	TEST_CHECK( !isIgnoredFile( "/tmp/file.cpp" ) );
}

//------------------------------------------------------------------------------

TEST_CASE( t5_2_ignore_files_library )
{
	// Init
	addFileFilter( "boost/.*" );

	// Check
	TEST_CHECK( isIgnoredFile( "boost/test/unit_test.hpp" ) );
	TEST_CHECK( isIgnoredFile( "boost\\test\\unit_test.hpp" ) );
	TEST_CHECK( !isIgnoredFile( "/tmp/file.cpp" ) );
}

//------------------------------------------------------------------------------

TEST_CASE( t5_3_ignore_file_name )
{
	// Init
	addFileFilter( "Q.*" );

	// Check
	TEST_CHECK( isIgnoredFile( "QWidget" ) );
	TEST_CHECK( !isIgnoredFile( "/tmp/q_file.cpp" ) );
}

//------------------------------------------------------------------------------

TEST_CASE( t5_4_ignore_sub_string )
{
	// Init
	addFileFilter( "/ui.*.h" );

	// Check
	TEST_CHECK( isIgnoredFile( "/tmp/ui/file.h" ) );
	TEST_CHECK( !isIgnoredFile( "/tmp/gui/file.h" ) );
}

//------------------------------------------------------------------------------

TEST_CASE( t5_5_ignore_only_begin_part )
{
	// Init
	addFileFilter( "^ui.*.h" );

	// Check
	TEST_CHECK( !isIgnoredFile( "/tmp/ui/file.h" ) );
	TEST_CHECK( !isIgnoredFile( "/tmp/gui/file.h" ) );
	TEST_CHECK( isIgnoredFile( "ui/file.h" ) );
}

//------------------------------------------------------------------------------

TEST_CASE( t5_6_invalid_regex )
{
	// Check
	TEST_CHECK_THROW( addFileFilter( "[" ), project::InvalidRegex );
}

//------------------------------------------------------------------------------

TEST_CASE( t5_7_double_plus )
{
	// Init
	addFileFilter( "lib\\+\\+v3/include/.*" );

	// Check
	TEST_CHECK( isIgnoredFile( "lib++v3/include/folder/file.h" ) );
}

//------------------------------------------------------------------------------

TEST_GROUP_END

//------------------------------------------------------------------------------

}
