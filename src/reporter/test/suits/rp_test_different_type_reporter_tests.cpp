#include "reporter/test/fixture/rp_test_reporter_fixture.hpp"

#include <boost/test/unit_test.hpp>

/*------------------------------------------------------------------------------

TEST PLAN:
1. Empty
2. One include
3. Several includes with same type
	3.1 user type
	3.2 system type
4. Several includes with different types
	4.1 system and user
	4.2 std
	4.3 hide std
5. Limits
	5.1 max files
	5.2 max details
6. Show only std headers

------------------------------------------------------------------------------*/

namespace reporter::test {

//------------------------------------------------------------------------------
// clazy:excludeall=non-pod-global-static

BOOST_FIXTURE_TEST_SUITE( DifferentTypeReporterTests, ReporterFixture )

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE( t1_empty )
{
	// Run
	const std::string result = runDifferentTypeReport();

	// Check
	BOOST_CHECK_EQUAL( result, "" );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE( t2_one_include )
{
	// Init
	addInclude( "/test_project/main.cpp", "/test_project/header.hpp" );

	// Run
	const std::string result = runDifferentTypeReport();

	// Check
	BOOST_CHECK_EQUAL( result, "" );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE( t3_1_several_includes_with_user_type )
{
	// Init
	addUserInclude( "/test_project/file1.cpp", "/test_project/header.hpp" );
	addUserInclude( "/test_project/file2.cpp", "/test_project/header.hpp" );

	// Run
	const std::string result = runDifferentTypeReport();

	// Check
	BOOST_CHECK_EQUAL( result, "" );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE( t3_2_several_includes_with_system_type )
{
	// Init
	addSystemInclude( "/test_project/file1.cpp", "/test_project/header.hpp" );
	addSystemInclude( "/test_project/file2.cpp", "/test_project/header.hpp" );

	// Run
	const std::string result = runDifferentTypeReport();

	// Check
	BOOST_CHECK_EQUAL( result, "" );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE( t4_1_several_includes_system_and_user )
{
	// Init
	setProjectDir( "/test_project/" );

	addUserInclude( "/test_project/file1.cpp", "/test_project/header.hpp" );
	addSystemInclude( "/test_project/file2.cpp", "/test_project/header.hpp" );

	// Run
	const std::string result = runDifferentTypeReport();

	// Check
	BOOST_CHECK_EQUAL(
		result,
		"Files that are included by different ways:\n"
		"1. header.hpp\n"
		"With double quotation marks ( #include \"...\" ) in files:\n"
			"\t1. file1.cpp line 1\n"
		"With angle brackets ( #include <...> ) in files:\n"
			"\t1. file2.cpp line 1\n"
	);
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE( t4_2_several_includes_std )
{
	// Init
	setProjectDir( "/test_project/" );

	addFile( "vector", model_includes::FileType::StdLibraryFile );

	addUserInclude(		"/test_project/file1.cpp", "vector" );
	addSystemInclude(	"/test_project/file2.cpp", "vector" );

	// Run
	setShowStdFiles( true );
	const std::string result = runDifferentTypeReport();

	// Check
	BOOST_CHECK_EQUAL(
		result,
		"Files that are included by different ways:\n"
		"1. vector\n"
		"With double quotation marks ( #include \"...\" ) in files:\n"
			"\t1. file1.cpp line 1\n"
		"With angle brackets ( #include <...> ) in files:\n"
			"\t1. file2.cpp line 1\n"
	);
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE( t4_3_several_includes_hide_std )
{
	// Init
	setProjectDir( "/test_project/" );

	addFile( "vector", model_includes::FileType::StdLibraryFile );

	addUserInclude( "/test_project/file1.cpp", "vector" );
	addSystemInclude( "/test_project/file2.cpp", "vector" );

	setShowStdFiles( false );

	// Run
	const std::string result = runDifferentTypeReport();

	// Check
	BOOST_CHECK_EQUAL( result, "" );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE( t5_1_limit_max_files )
{
	// Init
	setProjectDir( "/test_project/" );

	addUserInclude( "/test_project/file1.cpp", "/test_project/header1.hpp" );
	addSystemInclude( "/test_project/file2.cpp", "/test_project/header1.hpp" );

	addUserInclude( "/test_project/file1.cpp", "/test_project/header2.hpp" );
	addSystemInclude( "/test_project/file2.cpp", "/test_project/header2.hpp" );

	setMaxFilesCount( 1 );

	// Run
	const std::string result = runDifferentTypeReport();

	// Check
	BOOST_CHECK_EQUAL(
		result,
		"Files that are included by different ways:\n"
		"1. header1.hpp\n"
		"With double quotation marks ( #include \"...\" ) in files:\n"
			"\t1. file1.cpp line 1\n"
		"With angle brackets ( #include <...> ) in files:\n"
			"\t1. file2.cpp line 1\n"
		"... 1 of 2 files\n"
	);
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE( t5_2_limit_max_details )
{
	// Init
	setProjectDir( "/test_project/" );

	addUserInclude( "/test_project/file1.cpp", "/test_project/header1.hpp" );
	addUserInclude( "/test_project/file2.cpp", "/test_project/header1.hpp" );
	addSystemInclude( "/test_project/file3.cpp", "/test_project/header1.hpp" );
	addSystemInclude( "/test_project/file4.cpp", "/test_project/header1.hpp" );

	addUserInclude( "/test_project/file1.cpp", "/test_project/header2.hpp" );
	addUserInclude( "/test_project/file2.cpp", "/test_project/header2.hpp" );
	addSystemInclude( "/test_project/file3.cpp", "/test_project/header2.hpp" );
	addSystemInclude( "/test_project/file4.cpp", "/test_project/header2.hpp" );

	setMaxDetailsCount( 1 );

	// Run
	const std::string result = runDifferentTypeReport();

	// Check
	BOOST_CHECK_EQUAL(
		result,
		"Files that are included by different ways:\n"
		"1. header1.hpp\n"
		"With double quotation marks ( #include \"...\" ) in files:\n"
			"\t1. file1.cpp line 1\n"
			"\t... 1 of 2 details\n"
		"With angle brackets ( #include <...> ) in files:\n"
			"\t1. file3.cpp line 1\n"
			"\t... 1 of 2 details\n"
		"2. header2.hpp\n"
		"With double quotation marks ( #include \"...\" ) in files:\n"
			"\t1. file1.cpp line 1\n"
			"\t... 1 of 2 details\n"
		"With angle brackets ( #include <...> ) in files:\n"
			"\t1. file3.cpp line 1\n"
			"\t... 1 of 2 details\n"
	);
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE( t6_show_only_std_headers )
{
	// Init
	setProjectDir( "/test_project/" );

	addFile( "vector", model_includes::FileType::StdLibraryFile );
	addFile( "/test_project/header.hpp", model_includes::FileType::ProjectFile );

	addUserInclude( "/test_project/file1.cpp", "vector" );
	addSystemInclude( "/test_project/file2.cpp", "vector" );

	addUserInclude(	"/test_project/file1.cpp", "/test_project/header.hpp" );
	addSystemInclude( "/test_project/file2.cpp", "/test_project/header.hpp" );

	setShowOnlyStdHeaders( true );

	// Run
	const std::string result = runDifferentTypeReport();

	// Check
	BOOST_CHECK_EQUAL(
		result,
		"Files that are included by different ways:\n"
		"1. vector\n"
		"With double quotation marks ( #include \"...\" ) in files:\n"
			"\t1. file1.cpp line 1\n"
		"With angle brackets ( #include <...> ) in files:\n"
			"\t1. file2.cpp line 1\n"
	);
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE_END()

//------------------------------------------------------------------------------

}
