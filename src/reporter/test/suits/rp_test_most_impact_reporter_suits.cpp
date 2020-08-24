#include "reporter/test/fixture/rp_test_reporter_fixture.hpp"

#include <fmt/format.h>

#include <boost/test/unit_test.hpp>

/*------------------------------------------------------------------------------

TEST PLAN:
1. Empty
2. Only cpp files
3. Files with same count
4. Several details
5. Limit
	5.1 Max files
	5.2 Max details
	5.3 Limit equal to files number
	5.4 Limit equal to details number
6. Relevant paths

------------------------------------------------------------------------------*/

namespace reporter::test {

//------------------------------------------------------------------------------

BOOST_FIXTURE_TEST_SUITE(MostImpactReporterTests, ReporterFixture)

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(empty_1)
{
	// Run
	std::string result = runMostImpactReporter();

	// Check
	BOOST_CHECK_EQUAL( result, "" );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(only_cpp_files_2)
{
	// Init
	const std::string classAFile = "/test_project/classA.hpp";
	const std::string classBFile = "/test_project/classB.hpp";

	const std::string runA1file = "/test_project/runA1.cpp";
	const std::string runA2file = "/test_project/runA2.cpp";

	const std::string runB1file = "/test_project/runB1.cpp";

	addInclude( runA1file, classAFile );
	addInclude( runA2file, classAFile );

	addInclude( runB1file, classBFile );

	// Run
	std::string result = runMostImpactReporter();

	// Check
	BOOST_CHECK_EQUAL(
		result,
		"Most impact files:\n"
		"1 : \"" + toPath( classAFile ) + "\" impact to 2 file(s)\n"
		"Included by:\n"
			"\t1 : \"" + toPath( runA1file ) + "\" line 1\n"
			"\t2 : \"" + toPath( runA2file ) + "\" line 1\n"
		"2 : \"" + toPath( classBFile ) + "\" impact to 1 file(s)\n"
		"Included by:\n"
			"\t1 : \"" + toPath( runB1file ) +"\" line 1\n"
	);
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(files_with_same_count_3)
{
	// Init
	const std::string classAFile = "/test_project/classA.hpp";
	const std::string classBFile = "/test_project/classB.hpp";

	const std::string runA1file = "/test_project/runA1.cpp";
	const std::string runA2file = "/test_project/runA2.cpp";

	const std::string runB1file = "/test_project/runB1.cpp";
	const std::string runB2file = "/test_project/runB2.cpp";

	addInclude( runA1file, classAFile );
	addInclude( runA2file, classAFile );

	addInclude( runB1file, classBFile );
	addInclude( runB2file, classBFile );

	// Run
	std::string result = runMostImpactReporter();

	// Check
	BOOST_CHECK_EQUAL(
		result,
		"Most impact files:\n"
		"1 : \"" + toPath( classAFile ) + "\" impact to 2 file(s)\n"
		"Included by:\n"
			"\t1 : \"" + toPath( runA1file ) + "\" line 1\n"
			"\t2 : \"" + toPath( runA2file ) + "\" line 1\n"
		"2 : \"" + toPath( classBFile ) + "\" impact to 2 file(s)\n"
		"Included by:\n"
			"\t1 : \"" + toPath( runB1file ) +"\" line 1\n"
			"\t2 : \"" + toPath( runB2file ) +"\" line 1\n"
	);
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(several_details_4)
{
	// Init
	const std::string classABaseFile = "/test_project/classABase.hpp";
		const std::string classAFile = "/test_project/classA.hpp";
			const std::string runA1file = "/test_project/runA1.cpp";
			const std::string runA2file = "/test_project/runA2.cpp";

	const std::string classBBaseFile = "/test_project/classBBase.hpp";
		const std::string classBSubBaseFile = "/test_project/classBSubBase.hpp";
			const std::string classBFile = "/test_project/classB.hpp";
				const std::string runB1file = "/test_project/runB1.cpp";
				const std::string runB2file = "/test_project/runB2.cpp";

	addInclude( classAFile, classABaseFile );
		addInclude( runA1file, classAFile );
		addInclude( runA2file, classAFile );

	addInclude( classBSubBaseFile, classBBaseFile );
		addInclude( classBFile, classBSubBaseFile );
			addInclude( runB1file, classBFile );
			addInclude( runB2file, classBFile );

	// Run
	std::string result = runMostImpactReporter();

	// Check
	BOOST_CHECK_EQUAL(
		result,
		"Most impact files:\n"
		"1 : \"" + toPath( classBBaseFile ) + "\" impact to 4 file(s)\n"
		"Included by:\n"
			"\t1 : \"" + toPath( classBSubBaseFile ) + "\" line 1, impact to 3 file(s)\n"
		"2 : \"" + toPath( classABaseFile ) + "\" impact to 3 file(s)\n"
		"Included by:\n"
				"\t1 : \"" + toPath( classAFile ) + "\" line 1, impact to 2 file(s)\n"
		"3 : \"" + toPath( classBSubBaseFile ) + "\" impact to 3 file(s)\n"
		"Included by:\n"
			"\t1 : \"" + toPath( classBFile ) + "\" line 1, impact to 2 file(s)\n"
		"4 : \"" + toPath( classAFile ) + "\" impact to 2 file(s)\n"
		"Included by:\n"
			"\t1 : \"" + toPath( runA1file ) + "\" line 1\n"
			"\t2 : \"" + toPath( runA2file ) + "\" line 1\n"
		"5 : \"" + toPath( classBFile ) + "\" impact to 2 file(s)\n"
		"Included by:\n"
			"\t1 : \"" + toPath( runB1file ) +"\" line 1\n"
			"\t2 : \"" + toPath( runB2file ) +"\" line 1\n"

	);
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(limit_max_files_5_1)
{
	// Init
	const std::string sourceFileFmt		 = "/test_project/class{}.cpp";
	const std::string destinationFileFmt = "/test_project/class{}.hpp";

	const int filesCount = 10;
	for( int i = 0; i < filesCount; ++i )
	{
		const std::string sourceFile =		fmt::format( sourceFileFmt, i );
		const std::string destinationFile = fmt::format( destinationFileFmt, i );
		addInclude( sourceFile, destinationFile );
	}

	// Run
	const int limit = 5;
	BOOST_REQUIRE_LE( limit, filesCount );
	setMaxFilesCount( limit );
	std::string result = runMostImpactReporter();

	// Check
	std::string expectedString = "Most impact files:\n";
	const std::string lineFmt =
	"{} : \"{}\" impact to 1 file(s)\n"
	"Included by:\n"
		"\t1 : \"{}\" line 1\n"
	;
	for( int i = 0; i < limit; ++i )
	{
		const std::string sourceFile =		fmt::format( sourceFileFmt, i );
		const std::string destinationFile = fmt::format( destinationFileFmt, i );
		const std::string line = fmt::format(
			lineFmt,
			i + 1,
			toPath( destinationFile ),
			toPath( sourceFile )
		);
		expectedString += line;
	}

	expectedString += fmt::format( "... {} of {} files\n", limit, filesCount );

	BOOST_CHECK_EQUAL( result, expectedString );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(limit_max_details_5_2)
{
	// Init
	const std::string classBaseFile = "/test_project/classBase.hpp";
		const std::string classAFile = "/test_project/classA.hpp";
			const std::string runA1file = "/test_project/runA1.cpp";
			const std::string runA2file = "/test_project/runA2.cpp";

		const std::string classBFile = "/test_project/classB.hpp";
			const std::string runB1file = "/test_project/runB1.cpp";
			const std::string runB2file = "/test_project/runB2.cpp";

	addInclude( classAFile, classBaseFile );
		addInclude( runA1file, classAFile );
		addInclude( runA2file, classAFile );

	addInclude( classBFile, classBaseFile );
		addInclude( runB1file, classBFile );
		addInclude( runB2file, classBFile );

	setMaxDetailsCount( 1 );

	// Run
	std::string result = runMostImpactReporter();

	// Check
	BOOST_CHECK_EQUAL(
		result,
		"Most impact files:\n"
		"1 : \"" + toPath( classBaseFile ) + "\" impact to 6 file(s)\n"
		"Included by:\n"
			"\t1 : \"" + toPath( classAFile ) + "\" line 1, impact to 2 file(s)\n"
			"\t... 1 of 2 details\n"
		"2 : \"" + toPath( classAFile ) + "\" impact to 2 file(s)\n"
		"Included by:\n"
			"\t1 : \"" + toPath( runA1file ) + "\" line 1\n"
			"\t... 1 of 2 details\n"
		"3 : \"" + toPath( classBFile ) + "\" impact to 2 file(s)\n"
		"Included by:\n"
			"\t1 : \"" + toPath( runB1file ) + "\" line 1\n"
			"\t... 1 of 2 details\n"
	);
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(limit_equal_to_files_number_5_3)
{
	// Init
	const std::string sourceFileFmt		 = "/test_project/class{}.cpp";
	const std::string destinationFileFmt = "/test_project/class{}.hpp";

	const int filesCount = 10;
	for( int i = 0; i < filesCount; ++i )
	{
		const std::string sourceFile = fmt::format( sourceFileFmt, i );
		const std::string destinationFile = fmt::format( destinationFileFmt, i );
		addInclude( sourceFile, destinationFile );
	}

	// Run
	const int limit = filesCount;
	BOOST_REQUIRE_LE( limit, filesCount );
	setMaxFilesCount( limit );
	std::string result = runMostImpactReporter();

	// Check
	std::string expectedString = "Most impact files:\n";
	const std::string lineFmt =
	"{} : \"{}\" impact to 1 file(s)\n"
	"Included by:\n"
		"\t1 : \"{}\" line 1\n"
	;
	for( int i = 0; i < limit; ++i )
	{
		const std::string sourceFile =		fmt::format( sourceFileFmt, i );
		const std::string destinationFile = fmt::format( destinationFileFmt, i );
		const std::string line = fmt::format(
			lineFmt,
			i + 1,
			toPath( destinationFile ),
			toPath( sourceFile )
		);
		expectedString += line;
	}

	BOOST_CHECK_EQUAL( result, expectedString );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(limit_to_equal_details_number_5_4)
{
	// Init
	const std::string classBaseFile = "/test_project/classBase.hpp";
		const std::string classAFile = "/test_project/classA.hpp";
			const std::string runA1file = "/test_project/runA1.cpp";
			const std::string runA2file = "/test_project/runA2.cpp";

		const std::string classBFile = "/test_project/classB.hpp";
			const std::string runB1file = "/test_project/runB1.cpp";
			const std::string runB2file = "/test_project/runB2.cpp";

	addInclude( classAFile, classBaseFile );
		addInclude( runA1file, classAFile );
		addInclude( runA2file, classAFile );

	addInclude( classBFile, classBaseFile );
		addInclude( runB1file, classBFile );
		addInclude( runB2file, classBFile );

	setMaxDetailsCount( 2 );

	// Run
	std::string result = runMostImpactReporter();

	// Check
	BOOST_CHECK_EQUAL(
		result,
		"Most impact files:\n"
		"1 : \"" + toPath( classBaseFile ) + "\" impact to 6 file(s)\n"
		"Included by:\n"
			"\t1 : \"" + toPath( classAFile ) + "\" line 1, impact to 2 file(s)\n"
			"\t2 : \"" + toPath( classBFile ) + "\" line 1, impact to 2 file(s)\n"
		"2 : \"" + toPath( classAFile ) + "\" impact to 2 file(s)\n"
		"Included by:\n"
			"\t1 : \"" + toPath( runA1file ) + "\" line 1\n"
			"\t2 : \"" + toPath( runA2file ) + "\" line 1\n"
		"3 : \"" + toPath( classBFile ) + "\" impact to 2 file(s)\n"
			"Included by:\n"
			"\t1 : \"" + toPath( runB1file ) +"\" line 1\n"
			"\t2 : \"" + toPath( runB2file ) +"\" line 1\n"
	);
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(relevant_pants_6)
{
	// Init
	const std::string projectDir = "/test_project/";

	const std::string classAFileName = "classA.hpp";
	const std::string classAFile = projectDir + classAFileName;
	const std::string classBFileName = "classB.hpp";
	const std::string classBFile = projectDir + classBFileName;

	const std::string runA1fileName = "runA1.cpp";
	const std::string runA1file = projectDir + runA1fileName;
	const std::string runA2fileName = "runA2.cpp";
	const std::string runA2file = projectDir + runA2fileName;

	const std::string runB1fileName = "runB1.cpp";
	const std::string runB1file = projectDir + runB1fileName;

	setProjectDir( projectDir );

	addInclude( runA1file, classAFile );
	addInclude( runA2file, classAFile );

	addInclude( runB1file, classBFile );

	// Run
	std::string result = runMostImpactReporter();

	// Check
	BOOST_CHECK_EQUAL(
		result,
		"Most impact files:\n"
		"1 : \"" + toPath( classAFileName ) + "\" impact to 2 file(s)\n"
		"Included by:\n"
			"\t1 : \"" + toPath( runA1fileName ) + "\" line 1\n"
			"\t2 : \"" + toPath( runA2fileName ) + "\" line 1\n"
		"2 : \"" + toPath( classBFileName ) + "\" impact to 1 file(s)\n"
		"Included by:\n"
			"\t1 : \"" + toPath( runB1fileName ) +"\" line 1\n"
	);
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE_END()

//------------------------------------------------------------------------------

}
