#include "reporter/test/fixture/rp_test_reporter_fixture.hpp"

#include <boost/test/unit_test.hpp>

#include <set>

/*------------------------------------------------------------------------------

TEST PLAN:
1. Empty
2. One header
3. Several header
4. Extension
	4.1 hpp
	4.2 h
	4.3 Mix extensions
	4.4 empty
5. Limit
6. Thousands separator

------------------------------------------------------------------------------*/

namespace reporter::test
{
//------------------------------------------------------------------------------
// clazy:excludeall=non-pod-global-static
// NOLINTNEXTLINE(fuchsia-statically-constructed-objects,cert-err58-cpp)
BOOST_FIXTURE_TEST_SUITE( UnincludedReporterTests, ReporterFixture )

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE( t1_empty )
{
	// Init
	addInclude( "/test_project/main.cpp", "/test_project/classA.hpp" );
	addInclude( "/test_project/main.cpp", "/test_project/classB.hpp" );

	// Run
	std::string result = runUnincludedReporter();

	// Check
	BOOST_CHECK_EQUAL( result, "" );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE( t2_one_header )
{
	// Init
	setProjectDir( "/tmp/" );

	addFileToProject( "main.cpp" );
	addFileToProject( "header.hpp" );

	// Run
	std::string result = runUnincludedReporter();

	// Check
	BOOST_CHECK_EQUAL(
		result, "Unincluded header:\n"
				"1 : \"header.hpp\"\n" );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE( t3_several_headers )
{
	// Init
	setProjectDir( "/tmp/" );

	addFileToProject( "main.cpp" );
	addFileToProject( "classA.hpp" );
	addFileToProject( "classB.hpp" );
	addFileToProject( "classC.hpp" );

	// Run
	std::string result = runUnincludedReporter();

	// Check
	BOOST_CHECK_EQUAL(
		result, "Unincluded headers:\n"
				"1 : \"classA.hpp\"\n"
				"2 : \"classB.hpp\"\n"
				"3 : \"classC.hpp\"\n" );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE( t4_1_extension_hpp )
{
	// Init
	setProjectDir( "/tmp/" );

	addFileToProject( "main.cpp" );
	addFileToProject( "classA.hpp" );

	// Run
	std::string result = runUnincludedReporter();

	// Check
	BOOST_CHECK_EQUAL(
		result, "Unincluded header:\n"
				"1 : \"classA.hpp\"\n" );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE( t4_2_extension_h )
{
	// Init
	setProjectDir( "/tmp/" );

	addFileToProject( "main.cpp" );
	addFileToProject( "classA.h" );

	// Run
	std::string result = runUnincludedReporter();

	// Check
	BOOST_CHECK_EQUAL(
		result, "Unincluded header:\n"
				"1 : \"classA.h\"\n" );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE( t4_3_mix_extensions )
{
	// Init
	setProjectDir( "/tmp/" );

	addFileToProject( "main.cpp" );
	addFileToProject( "classA.h" );
	addFileToProject( "classB.hpp" );

	// Run
	std::string result = runUnincludedReporter();

	// Check
	BOOST_CHECK_EQUAL(
		result, "Unincluded headers:\n"
				"1 : \"classA.h\"\n"
				"2 : \"classB.hpp\"\n" );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE( t4_4_empty_extension )
{
	// Init
	addFileToProject( "main.cpp" );
	addFileToProject( "header" );

	// Run
	std::string result = runUnincludedReporter();

	// Check
	BOOST_CHECK_EQUAL(
		result, "Unincluded header:\n"
				"1 : \"header\"\n" );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE( t5_limit )
{
	// Init
	setProjectDir( "/tmp/" );

	addFileToProject( "main.cpp" );
	addFileToProject( "classA.hpp" );
	addFileToProject( "classB.hpp" );
	addFileToProject( "classC.hpp" );

	setMaxFilesCount( 2 );

	// Run
	std::string result = runUnincludedReporter();

	// Check
	BOOST_CHECK_EQUAL(
		result, "Unincluded headers:\n"
				"1 : \"classA.hpp\"\n"
				"2 : \"classB.hpp\"\n"
				"... 2 of 3 files\n" );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE( t6_thousands_separator )
{
	// Init
	setProjectDir( "/tmp/" );

	addFileToProject( "main.cpp" );

	const int count = 1'100;
	for( int i = 1; i <= count; ++i )
	{
		addFileToProject( "header" + std::to_string( i ) + ".hpp" );
	}

	setSystemThousandsSeparator( ',' );

	// Run
	std::string result = runUnincludedReporter();

	// Check

	std::set< std::string > orderedFiles;
	for( int i = 1; i <= count; ++i )
	{
		std::string fileName = "header" + std::to_string( i ) + ".hpp";
		orderedFiles.insert( fileName );
	}

	std::string expectedResult = "Unincluded headers:\n";

	int number = 1;
	for( const std::string & str: orderedFiles )
	{
		std::string currentNumber =
			( number >= 1'000 ? std::to_string( number ).insert( 1, 1, ',' )
							  : std::to_string( number ) );

		expectedResult += currentNumber + " : \"" + str + "\"\n";
		++number;
	}

	BOOST_CHECK_EQUAL( result, expectedResult );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE_END()

//------------------------------------------------------------------------------

}
