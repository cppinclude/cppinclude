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

BOOST_AUTO_TEST_CASE(t1_1_project_dir_absolute_path)
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

BOOST_AUTO_TEST_CASE(t1_2_project_dir_relative_path)
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

BOOST_AUTO_TEST_CASE(t2_1_include_dirs_absolute_paths)
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

BOOST_AUTO_TEST_CASE(t2_2_include_dirs_relative_paths)
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

BOOST_AUTO_TEST_CASE(t3_1_ignore_dirs_absolute_paths)
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

BOOST_AUTO_TEST_CASE(t3_2_ignore_dirs_relative_paths)
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

BOOST_AUTO_TEST_CASE(t3_3_ignore_dirs_is_ignore)
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

BOOST_AUTO_TEST_CASE(t4_1_cpp_extension_is_cpp_extension)
{
	// Run
	addCppExtensions( { "*.cpp", "*.hpp" } );

	//Check
	BOOST_CHECK( isExistsCppExtension( "*.cpp" ) );
	BOOST_CHECK( isExistsCppExtension( "*.hpp" ) );
	BOOST_CHECK( isExistsCppExtension( ".hpp" ) );
	BOOST_CHECK( !isExistsCppExtension( ".cxx" ) );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(t5_1_ignore_files_empty)
{
	// Init
	changeAllPathsToAbsolute(); // create project

	//Check
	BOOST_CHECK( !isIgnoredFile( "/tmp/file.cpp" ) );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(t5_2_ignore_files_library)
{
	// Init
	addFileFilter( "boost/.*" );

	//Check
	BOOST_CHECK( isIgnoredFile( "boost/test/unit_test.hpp" ) );
	BOOST_CHECK( isIgnoredFile( "boost\\test\\unit_test.hpp" ) );
	BOOST_CHECK( !isIgnoredFile( "/tmp/file.cpp" ) );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(t5_3_file_name_5_3)
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
