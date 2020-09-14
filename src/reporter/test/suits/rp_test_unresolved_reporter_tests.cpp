#include "reporter/test/fixture/rp_test_reporter_fixture.hpp"

#include <boost/test/unit_test.hpp>

/*------------------------------------------------------------------------------

TEST PLAN:
1. Model doesn't have unresolved includes
2. Several includes
3. Relative paths
4. Limits
	4.1 Max files
	4.2 Max details
	4.3 Limit equal to files count
	4.4 Limit equal to details count
5. Ordering

------------------------------------------------------------------------------*/

namespace reporter::test {

//------------------------------------------------------------------------------

BOOST_FIXTURE_TEST_SUITE(UnresolvedReporterTests, ReporterFixture)

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(t1_model_not_have_unresolved_includes)
{
	// Init
	addInclude( "/test_project/main.cpp", "/test_project/classA.hpp" );
	addInclude( "/test_project/main.cpp", "/test_project/classB.hpp" );

	// Run
	std::string result = runUnresolvedReporter();

	// Check
	BOOST_CHECK_EQUAL( result, "" );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(t2_several_includes)
{
	// Init
	const auto unresolvedStatus = IncludeStatus::Unresolved;

	const std::string classAFile = "/test_project/classA.hpp";
	const std::string classBFile = "/test_project/classB.hpp";

	const std::string runA1file = "/test_project/runA1.cpp";
	const std::string runA2file = "/test_project/runA2.cpp";

	const std::string runB1file = "/test_project/runB1.cpp";
	const std::string runB2file = "/test_project/runB2.cpp";

	addInclude( runA1file, classAFile, unresolvedStatus );
	addInclude( runA2file, classAFile, unresolvedStatus );

	addInclude( runB1file, classBFile, unresolvedStatus );
	addInclude( runB2file, classBFile, unresolvedStatus );

	// Run
	std::string result = runUnresolvedReporter();

	// Check
	BOOST_CHECK_EQUAL(
		result,
		"Unresolved files:\n"
		"1. \"" + toPath( classAFile )+ "\" isn't resolved in:\n"
			"\t1. \"" + toPath( runA1file ) + "\" line: 1\n" +
			"\t2. \"" + toPath( runA2file ) + "\" line: 1\n" +
		"2. \"" + toPath( classBFile ) + "\" isn't resolved in:\n" +
			"\t1. \"" + toPath( runB1file )+ "\" line: 1\n" +
			"\t2. \"" + toPath( runB2file ) + "\" line: 1\n"
	);
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(t3_relative_paths)
{
	// Init
	const auto unresolvedStatus = IncludeStatus::Unresolved;

	const std::string projectDir = "/test_project/";

	const std::string classAFileName = "classA.hpp";
	const std::string classAFile = projectDir + classAFileName;
	const std::string classBFileName = "classB.hpp";
	const std::string classBFile = projectDir + classBFileName;;

	const std::string runA1fileName = "runA1.cpp";
	const std::string runA1file = projectDir + runA1fileName;
	const std::string runA2fileName = "runA2.cpp";
	const std::string runA2file = projectDir + runA2fileName;

	const std::string runB1fileName = "runB1.cpp";
	const std::string runB1file = projectDir + runB1fileName;
	const std::string runB2fileName = "runB2.cpp";
	const std::string runB2file = projectDir + runB2fileName;

	setProjectDir( projectDir );

	addInclude( runA1file, classAFile, unresolvedStatus );
	addInclude( runA2file, classAFile, unresolvedStatus );

	addInclude( runB1file, classBFile, unresolvedStatus );
	addInclude( runB2file, classBFile, unresolvedStatus );

	// Run
	std::string result = runUnresolvedReporter();

	// Check
	BOOST_CHECK_EQUAL(
		result,
		"Unresolved files:\n"
		"1. \"" + classAFileName + "\" isn't resolved in:\n"
			"\t1. \"" + runA1fileName + "\" line: 1\n" +
			"\t2. \"" + runA2fileName + "\" line: 1\n" +
		"2. \"" + classBFileName + "\" isn't resolved in:\n" +
			"\t1. \"" + runB1fileName + "\" line: 1\n" +
			"\t2. \"" + runB2fileName + "\" line: 1\n"
	);
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(t4_1_limit_max_files)
{
	// Init
	const int limit = 1;
	const auto unresolvedStatus = IncludeStatus::Unresolved;

	const std::string classAFile = "/test_project/classA.hpp";
	const std::string classBFile = "/test_project/classB.hpp";

	const std::string runA1file = "/test_project/runA1.cpp";
	const std::string runA2file = "/test_project/runA2.cpp";

	const std::string runB1file = "/test_project/runB1.cpp";
	const std::string runB2file = "/test_project/runB2.cpp";

	addInclude( runA1file, classAFile, unresolvedStatus );
	addInclude( runA2file, classAFile, unresolvedStatus );

	addInclude( runB1file, classBFile, unresolvedStatus );
	addInclude( runB2file, classBFile, unresolvedStatus );

	// Run
	setMaxFilesCount( limit );
	std::string result = runUnresolvedReporter();

	// Check
	BOOST_CHECK_EQUAL(
		result,
		"Unresolved files:\n"
		"1. \"" + toPath( classAFile ) + "\" isn't resolved in:\n"
			"\t1. \"" + toPath( runA1file ) + "\" line: 1\n" +
			"\t2. \"" + toPath( runA2file ) + "\" line: 1\n" +
		"... 1 of 2 files\n"
	);
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(t4_2_limit_max_details)
{
	// Init
	const int limit = 1;
	const auto unresolvedStatus = IncludeStatus::Unresolved;

	const std::string classAFile = "/test_project/classA.hpp";

	const std::string runA1file = "/test_project/runA1.cpp";
	const std::string runA2file = "/test_project/runA2.cpp";

	addInclude( runA1file, classAFile, unresolvedStatus );
	addInclude( runA2file, classAFile, unresolvedStatus );

	// Run
	setMaxDetailsCount( limit );
	std::string result = runUnresolvedReporter();

	// Check
	BOOST_CHECK_EQUAL(
		result,
		"Unresolved files:\n"
		"1. \"" + toPath( classAFile ) + "\" isn't resolved in:\n"
			"\t1. \"" + toPath( runA1file ) + "\" line: 1\n" +
			"\t... 1 of 2 details\n"
	);
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(t4_3_limit_equal_to_files_count)
{
	// Init
	const int limit = 2;
	const auto unresolvedStatus = IncludeStatus::Unresolved;

	const std::string classAFile = "/test_project/classA.hpp";
	const std::string classBFile = "/test_project/classB.hpp";

	const std::string runA1file = "/test_project/runA1.cpp";
	const std::string runA2file = "/test_project/runA2.cpp";

	const std::string runB1file = "/test_project/runB1.cpp";
	const std::string runB2file = "/test_project/runB2.cpp";

	addInclude( runA1file, classAFile, unresolvedStatus );
	addInclude( runA2file, classAFile, unresolvedStatus );

	addInclude( runB1file, classBFile, unresolvedStatus );
	addInclude( runB2file, classBFile, unresolvedStatus );

	// Run
	setMaxFilesCount( limit );
	std::string result = runUnresolvedReporter();

	// Check
	BOOST_CHECK_EQUAL(
		result,
		"Unresolved files:\n"
		"1. \"" + toPath( classAFile ) + "\" isn't resolved in:\n"
			"\t1. \"" + toPath( runA1file ) + "\" line: 1\n" +
			"\t2. \"" + toPath( runA2file ) + "\" line: 1\n"
		"2. \"" + toPath( classBFile ) + "\" isn't resolved in:\n"
			"\t1. \"" + toPath( runB1file ) + "\" line: 1\n" +
			"\t2. \"" + toPath( runB2file ) + "\" line: 1\n"
	);
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(t4_4_limit_equal_to_details_count)
{
	// Init
	const int limit = 2;
	const auto unresolvedStatus = IncludeStatus::Unresolved;

	const std::string classAFile = "/test_project/classA.hpp";

	const std::string runA1file = "/test_project/runA1.cpp";
	const std::string runA2file = "/test_project/runA2.cpp";

	addInclude( runA1file, classAFile, unresolvedStatus );
	addInclude( runA2file, classAFile, unresolvedStatus );

	// Run
	setMaxDetailsCount( limit );
	std::string result = runUnresolvedReporter();

	// Check
	BOOST_CHECK_EQUAL(
		result,
		"Unresolved files:\n"
		"1. \"" + toPath( classAFile ) + "\" isn't resolved in:\n"
			"\t1. \"" + toPath( runA1file ) + "\" line: 1\n"
			"\t2. \"" + toPath( runA2file ) + "\" line: 1\n"
	);
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(t5_ordering)
{
	// Init
	const auto unresolvedStatus = IncludeStatus::Unresolved;

	const std::string classAFile = "/test_project/classA.hpp";
	const std::string classBFile = "/test_project/classB.hpp";
	const std::string classCFile = "/test_project/classC.hpp";

	const std::string runA1file = "/test_project/runA1.cpp";
	const std::string runA2file = "/test_project/runA2.cpp";

	const std::string runB1file = "/test_project/runB1.cpp";
	const std::string runB2file = "/test_project/runB2.cpp";

	const std::string runC1file = "/test_project/runC1.cpp";
	const std::string runC2file = "/test_project/runC2.cpp";
	const std::string runC3file = "/test_project/runC3.cpp";

	addInclude( runA1file, classAFile, unresolvedStatus );
	addInclude( runA2file, classAFile, unresolvedStatus );

	addInclude( runB1file, classBFile, unresolvedStatus );
	addInclude( runB2file, classBFile, unresolvedStatus );

	addInclude( runC1file, classCFile, unresolvedStatus );
	addInclude( runC2file, classCFile, unresolvedStatus );
	addInclude( runC3file, classCFile, unresolvedStatus );

	// Run
	std::string result = runUnresolvedReporter();

	// Check
	BOOST_CHECK_EQUAL(
		result,
		"Unresolved files:\n"
		"1. \"" + toPath( classCFile ) + "\" isn't resolved in:\n"
			"\t1. \"" + toPath( runC1file ) + "\" line: 1\n" +
			"\t2. \"" + toPath( runC2file ) + "\" line: 1\n" +
			"\t3. \"" + toPath( runC3file ) + "\" line: 1\n" +
		"2. \"" + toPath( classAFile )+ "\" isn't resolved in:\n"
			"\t1. \"" + toPath( runA1file ) + "\" line: 1\n" +
			"\t2. \"" + toPath( runA2file ) + "\" line: 1\n" +
		"3. \"" + toPath( classBFile ) + "\" isn't resolved in:\n" +
			"\t1. \"" + toPath( runB1file ) + "\" line: 1\n" +
			"\t2. \"" + toPath( runB2file ) + "\" line: 1\n"
	);
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE_END()

//------------------------------------------------------------------------------

}
