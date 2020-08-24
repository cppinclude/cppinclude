#include "project/test/fixture/prj_test_ficture.hpp"

#include <boost/test/unit_test.hpp>
#include <boost/test/test_tools.hpp>

#include <filesystem>

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


------------------------------------------------------------------------------*/

namespace project::test {

//------------------------------------------------------------------------------

BOOST_FIXTURE_TEST_SUITE(ProjectTests, ProjectFixture)

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(project_dir_absolute_path_1_1)
{
	// Init
	const Path projectDirFullPath = "/home/user/project/";
	setProjectDir( projectDirFullPath );

	//Run
	changeAllPathsToAbsolute();

	//Check
	BOOST_CHECK_EQUAL( getProjectDir(), projectDirFullPath );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(project_dir_relative_path_1_2)
{
	// Init
	const Path projectRelativePath = "project/";
	const Path projectAbsolutePath = toAbsolutePath( projectRelativePath );

	setProjectDir( projectRelativePath );

	//Run
	changeAllPathsToAbsolute();

	//Check
	BOOST_CHECK_EQUAL( getProjectDir(), projectAbsolutePath );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(include_dirs_absolute_paths_2_1)
{
	// Init
	setProjectDir( "/home/user/project/" );
	addIncludeDirs( { "/usr/lib1", "/usr/lib2" } );

	//Run
	changeAllPathsToAbsolute();

	//Check
	Paths currentPaths = getIncludeDirs();
	Paths exceptedPaths{ "/usr/lib1/","/usr/lib2/" };

	BOOST_CHECK_EQUAL_COLLECTIONS(
		currentPaths.begin(),
		currentPaths.end(),

		exceptedPaths.begin(),
		exceptedPaths.end()
	);

}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(include_dirs_relative_paths_2_2)
{
	// Init
	setProjectDir( "/project/" );
	addIncludeDirs( { "lib1", "lib2" } );

	//Run
	changeAllPathsToAbsolute();

	//Check
	Paths currentPaths = getIncludeDirs();
	Paths exceptedPaths{ "/project/lib1/","/project/lib2/" };

	BOOST_CHECK_EQUAL_COLLECTIONS(
		currentPaths.begin(),
		currentPaths.end(),

		exceptedPaths.begin(),
		exceptedPaths.end()
	);
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(ignore_dirs_absolute_paths_3_1)
{
	// Init
	setProjectDir( "/home/user/project/" );
	addIgnoreDirs( { "/usr/ignore1", "/usr/ignore2" } );

	//Run
	changeAllPathsToAbsolute();

	//Check
	Paths currentPaths = getIgnoreDirs();
	Paths exceptedPaths{ "/usr/ignore1/", "/usr/ignore2/" };

	BOOST_CHECK_EQUAL_COLLECTIONS(
		currentPaths.begin(),
		currentPaths.end(),

		exceptedPaths.begin(),
		exceptedPaths.end()
	);
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(ignore_dirs_relative_paths_3_2)
{
	// Init
	setProjectDir( "/project/" );
	addIgnoreDirs( { "ignore1", "ignore2" } );

	//Run
	changeAllPathsToAbsolute();

	//Check
	Paths currentPaths = getIgnoreDirs();
	Paths exceptedPaths{ "/project/ignore1/", "/project/ignore2/" };

	BOOST_CHECK_EQUAL_COLLECTIONS(
		currentPaths.begin(),
		currentPaths.end(),

		exceptedPaths.begin(),
		exceptedPaths.end()
	);
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(ignore_dirs_is_ignore_3_3)
{
	// Init

	setProjectDir( "/project/" );
	addIgnoreDirs( { "ignore1", "/usr/ignore2" } );

	//Run
	changeAllPathsToAbsolute();

	//Check
	BOOST_CHECK( isIgnoreDir( "/project/ignore1" ) );
	BOOST_CHECK( isIgnoreDir( "/usr/ignore2" ) );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(cpp_extention_is_cpp_extention_4_1)
{
	// Run
	addCppExtentions( { "*.cpp", "*.hpp" } );

	//Check
	BOOST_CHECK( isExistsCppExtention( "*.cpp" ) );
	BOOST_CHECK( isExistsCppExtention( "*.hpp" ) );
	BOOST_CHECK( isExistsCppExtention( ".hpp" ) );
	BOOST_CHECK( !isExistsCppExtention( ".cxx" ) );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(ignore_files_empty_5_1)
{
	// Init
	changeAllPathsToAbsolute(); // create project

	//Check
	BOOST_CHECK( !isIgnoredFile( "/tmp/file.cpp" ) );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(ignore_files_library_5_2)
{
	// Init
	addFileFilter( "boost/.*" );

	//Check
	BOOST_CHECK( isIgnoredFile( "boost/test/unit_test.hpp" ) );
	BOOST_CHECK( isIgnoredFile( "boost\\test\\unit_test.hpp" ) );
	BOOST_CHECK( !isIgnoredFile( "/tmp/file.cpp" ) );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(file_name_5_3)
{
	// Init
	addFileFilter( "Q.*" );

	//Check
	BOOST_CHECK( isIgnoredFile( "QWidget" ) );
	BOOST_CHECK( !isIgnoredFile( "/tmp/q_file.cpp" ) );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE_END()

//------------------------------------------------------------------------------

}
