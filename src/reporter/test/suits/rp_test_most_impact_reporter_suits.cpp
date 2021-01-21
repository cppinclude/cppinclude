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
7. Std Files
	7.1 Don't show
	7.2 Show
8. Files out of project
	8.1 in sub directory
	8.2 in other directory

------------------------------------------------------------------------------*/

namespace reporter::test {

//------------------------------------------------------------------------------

BOOST_FIXTURE_TEST_SUITE(MostImpactReporterTests, ReporterFixture)

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(t1_empty)
{
	// Run
	std::string result = runMostImpactReporter();

	// Check
	BOOST_CHECK_EQUAL( result, "" );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(t2_only_cpp_files)
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
		"1 : \"" + toPath( classAFile ) + "\" impact on 2 file(s)\n"
		"Included by:\n"
			"\t1 : \"" + toPath( runA1file ) + "\" line 1\n"
			"\t2 : \"" + toPath( runA2file ) + "\" line 1\n"
		"2 : \"" + toPath( classBFile ) + "\" impact on 1 file(s)\n"
		"Included by:\n"
			"\t1 : \"" + toPath( runB1file ) +"\" line 1\n"
	);
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(t3_files_with_same_count)
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
		"1 : \"" + toPath( classAFile ) + "\" impact on 2 file(s)\n"
		"Included by:\n"
			"\t1 : \"" + toPath( runA1file ) + "\" line 1\n"
			"\t2 : \"" + toPath( runA2file ) + "\" line 1\n"
		"2 : \"" + toPath( classBFile ) + "\" impact on 2 file(s)\n"
		"Included by:\n"
			"\t1 : \"" + toPath( runB1file ) +"\" line 1\n"
			"\t2 : \"" + toPath( runB2file ) +"\" line 1\n"
	);
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(t4_several_details)
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
		"1 : \"" + toPath( classBBaseFile ) + "\" impact on 4 file(s)\n"
		"Included by:\n"
			"\t1 : \"" + toPath( classBSubBaseFile ) + "\" line 1, impact on 3 file(s)\n"
		"2 : \"" + toPath( classABaseFile ) + "\" impact on 3 file(s)\n"
		"Included by:\n"
				"\t1 : \"" + toPath( classAFile ) + "\" line 1, impact on 2 file(s)\n"
		"3 : \"" + toPath( classBSubBaseFile ) + "\" impact on 3 file(s)\n"
		"Included by:\n"
			"\t1 : \"" + toPath( classBFile ) + "\" line 1, impact on 2 file(s)\n"
		"4 : \"" + toPath( classAFile ) + "\" impact on 2 file(s)\n"
		"Included by:\n"
			"\t1 : \"" + toPath( runA1file ) + "\" line 1\n"
			"\t2 : \"" + toPath( runA2file ) + "\" line 1\n"
		"5 : \"" + toPath( classBFile ) + "\" impact on 2 file(s)\n"
		"Included by:\n"
			"\t1 : \"" + toPath( runB1file ) +"\" line 1\n"
			"\t2 : \"" + toPath( runB2file ) +"\" line 1\n"

	);
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(t5_1_limit_max_files)
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
	"{} : \"{}\" impact on 1 file(s)\n"
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

BOOST_AUTO_TEST_CASE(t5_2_limit_max_details)
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
		"1 : \"" + toPath( classBaseFile ) + "\" impact on 6 file(s)\n"
		"Included by:\n"
			"\t1 : \"" + toPath( classAFile ) + "\" line 1, impact on 2 file(s)\n"
			"\t... 1 of 2 details\n"
		"2 : \"" + toPath( classAFile ) + "\" impact on 2 file(s)\n"
		"Included by:\n"
			"\t1 : \"" + toPath( runA1file ) + "\" line 1\n"
			"\t... 1 of 2 details\n"
		"3 : \"" + toPath( classBFile ) + "\" impact on 2 file(s)\n"
		"Included by:\n"
			"\t1 : \"" + toPath( runB1file ) + "\" line 1\n"
			"\t... 1 of 2 details\n"
	);
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(t5_3_limit_equal_to_files_number)
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
	"{} : \"{}\" impact on 1 file(s)\n"
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

BOOST_AUTO_TEST_CASE(t5_4_limit_to_equal_details_number)
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
		"1 : \"" + toPath( classBaseFile ) + "\" impact on 6 file(s)\n"
		"Included by:\n"
			"\t1 : \"" + toPath( classAFile ) + "\" line 1, impact on 2 file(s)\n"
			"\t2 : \"" + toPath( classBFile ) + "\" line 1, impact on 2 file(s)\n"
		"2 : \"" + toPath( classAFile ) + "\" impact on 2 file(s)\n"
		"Included by:\n"
			"\t1 : \"" + toPath( runA1file ) + "\" line 1\n"
			"\t2 : \"" + toPath( runA2file ) + "\" line 1\n"
		"3 : \"" + toPath( classBFile ) + "\" impact on 2 file(s)\n"
			"Included by:\n"
			"\t1 : \"" + toPath( runB1file ) +"\" line 1\n"
			"\t2 : \"" + toPath( runB2file ) +"\" line 1\n"
	);
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(t6_relevant_pants)
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
		"1 : \"" + toPath( classAFileName ) + "\" impact on 2 file(s)\n"
		"Included by:\n"
			"\t1 : \"" + toPath( runA1fileName ) + "\" line 1\n"
			"\t2 : \"" + toPath( runA2fileName ) + "\" line 1\n"
		"2 : \"" + toPath( classBFileName ) + "\" impact on 1 file(s)\n"
		"Included by:\n"
			"\t1 : \"" + toPath( runB1fileName ) +"\" line 1\n"
	);
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(t7_1_std_file_dont_show)
{
	// Init
	const std::string stdFile	= "vector";

	const std::string classAFile = "/test_project/classA.hpp";
	const std::string classBFile = "/test_project/classB.hpp";

	const std::string runA1file = "/test_project/runA1.cpp";
	const std::string runA2file = "/test_project/runA2.cpp";

	const std::string runB1file = "/test_project/runB1.cpp";

	addFile( "vector", FileType::StdLibraryFile );

	addInclude( classAFile, stdFile, IncludeType::System );
	addInclude( classBFile, stdFile, IncludeType::System );

	addInclude( runA1file, classAFile );
	addInclude( runA2file, classAFile );

	addInclude( runB1file, classBFile );

	setShowStdFiles( false );

	// Run
	std::string result = runMostImpactReporter();

	// Check
	BOOST_CHECK_EQUAL(
		result,
		"Most impact files:\n"
		"1 : \"" + toPath( classAFile ) + "\" impact on 2 file(s)\n"
		"Included by:\n"
			"\t1 : \"" + toPath( runA1file ) + "\" line 1\n"
			"\t2 : \"" + toPath( runA2file ) + "\" line 1\n"
		"2 : \"" + toPath( classBFile ) + "\" impact on 1 file(s)\n"
		"Included by:\n"
			"\t1 : \"" + toPath( runB1file ) +"\" line 1\n"
	);
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(t7_2_std_file_show)
{
	// Init
	const std::string stdFile	= "vector";

	const std::string classAFile = "/test_project/classA.hpp";
	const std::string classBFile = "/test_project/classB.hpp";

	const std::string runA1file = "/test_project/runA1.cpp";
	const std::string runA2file = "/test_project/runA2.cpp";

	const std::string runB1file = "/test_project/runB1.cpp";

	addFile( "vector", FileType::StdLibraryFile );

	addInclude( classAFile, stdFile, IncludeType::System );
	addInclude( classBFile, stdFile, IncludeType::System );

	addInclude( runA1file, classAFile );
	addInclude( runA2file, classAFile );

	addInclude( runB1file, classBFile );

	setShowStdFiles( true );

	// Run
	std::string result = runMostImpactReporter();

	// Check
	BOOST_CHECK_EQUAL(
		result,
		"Most impact files:\n"
		"1 : \"" + stdFile + "\" impact on 5 file(s)\n"
		"Included by:\n"
			"\t1 : \"" + toPath( classAFile ) + "\" line 1, impact on 2 file(s)\n"
			"\t2 : \"" + toPath( classBFile ) + "\" line 1, impact on 1 file(s)\n"
		"2 : \"" + toPath( classAFile ) + "\" impact on 2 file(s)\n"
		"Included by:\n"
			"\t1 : \"" + toPath( runA1file ) + "\" line 1\n"
			"\t2 : \"" + toPath( runA2file ) + "\" line 1\n"
		"3 : \"" + toPath( classBFile ) + "\" impact on 1 file(s)\n"
		"Included by:\n"
			"\t1 : \"" + toPath( runB1file ) +"\" line 1\n"
	);
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(t8_1_files_out_of_project_in_subdir)
{
	// Init
	const std::string projectDir = "/test_project/";
	const std::string subdir = "build/";

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

	setProjectDir( projectDir + subdir );

	addInclude( runA1file, classAFile );
	addInclude( runA2file, classAFile );

	addInclude( runB1file, classBFile );

	// Run
	std::string result = runMostImpactReporter();

	// Check
	BOOST_CHECK_EQUAL(
		result,
		"Most impact files:\n"
		"1 : \"" + toPath( "../" + classAFileName ) + "\" impact on 2 file(s)\n"
		"Included by:\n"
			"\t1 : \"" + toPath( "../" + runA1fileName ) + "\" line 1\n"
			"\t2 : \"" + toPath( "../" + runA2fileName ) + "\" line 1\n"
		"2 : \"" + toPath( "../" + classBFileName ) + "\" impact on 1 file(s)\n"
		"Included by:\n"
			"\t1 : \"" + toPath( "../" + runB1fileName ) +"\" line 1\n"
	);
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(t8_2_files_out_of_project_in_other_dir)
{
	// Init
	const std::string projectDir = "/test_project/";
	const std::string otherDir = "/tmp/";

	const std::string classAFileName = "classA.hpp";
	const std::string classAFile = otherDir + classAFileName;
	const std::string classBFileName = "classB.hpp";
	const std::string classBFile = otherDir + classBFileName;

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
		"1 : \"" + toPath( classAFile ) + "\" impact on 2 file(s)\n"
		"Included by:\n"
			"\t1 : \"" + toPath( runA1fileName ) + "\" line 1\n"
			"\t2 : \"" + toPath( runA2fileName ) + "\" line 1\n"
		"2 : \"" + toPath( classBFile ) + "\" impact on 1 file(s)\n"
		"Included by:\n"
			"\t1 : \"" + toPath( runB1fileName ) +"\" line 1\n"
	);
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE_END()

//------------------------------------------------------------------------------

}
