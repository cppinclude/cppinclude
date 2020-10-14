#include "application/test/fixtures/app_test_parser_wrapper_fixture.hpp"

#include "application/resources/app_resources_arguments.hpp"
#include "application/exceptions/app_exceptions.hpp"

#include "reporter/api/enums/rp_reporter_kind.hpp"

#include <boost/test/unit_test.hpp>

#include <std_fs>
#include <optional>

/*------------------------------------------------------------------------------

TEST PLAN

1. Project dir
	1.1 From arguments
	1.2 Default
	1.3 From argument without full name
2. File extensions
	2.1 From arguments
	2.2 Default
3. Include dirs
	3.1 From arguments
	3.2 Default
4. Ignore dirs
	4.1 From arguments
	4.2 Default
5. Configuration file
	5.1 From arguments
	5.2 Default
6. Help
	6.1 From arguments
7. Verbose
8. Report
	8.1 From arguments
	8.2 Default
	8.3 Incorrect name
9. Report limit
	9.1 From arguments
	9.2 Default
10. Ignore system includes
	10.1 From arguments
	10.2 Default
11. Ignore files
	11.1 From arguments
	11.2 Default
12. Version
13. Report details limit
	13.1 From arguments
	13.2 Default
14. Analyze without extension
	14.1 From arguments
	14.2 Default
15. Show std files
	15.1 From arguments
	15.2 Default

-------------------------------------------------------------------------------*/

namespace application::test {

//------------------------------------------------------------------------------

BOOST_FIXTURE_TEST_SUITE(ParserWrapperTets, ParserWrapperFixture)

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(t1_1_project_dir_from_argument)
{
	// Init
    const std::string argumentName = resources::arguments::projectDir::FullName;
    const std::string argumentValue = "test_value";

    const std::string argument = "--" + argumentName + "=" + argumentValue;

	// Run
    parse({ argument });

	// Check
	BOOST_CHECK_EQUAL( argumentValue, getProjectDir() );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(t1_2_project_dir_default)
{
    // Run
    parse({ "" });

    // Check
    BOOST_CHECK_EQUAL(
        resources::arguments::projectDir::DefaultValue,
        getDefaultProjectDir()
    );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(t1_3_project_dir_from_argument_without_full_name)
{
	// Init
	const std::string projectDir = "project_dir";

	// Run
	parse({ projectDir });

	// Check
	BOOST_CHECK_EQUAL( projectDir, getProjectDir() );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(t2_1_file_extensions_from_argument)
{
    // Init
	const std::string argumentName = resources::arguments::fileExtensions::FullName;
    const std::string argumentValue = "*.cpp,*.hpp";

    const std::string argument = "--" + argumentName + "=" + argumentValue;

    // Run
    parse({ argument });

    // Check
	auto fileExtensions = getFileExtensions();
	BOOST_REQUIRE_EQUAL( fileExtensions.size(), 2 );
	BOOST_CHECK_EQUAL(fileExtensions.at( 0 ), "*.cpp");
	BOOST_CHECK_EQUAL(fileExtensions.at( 1 ), "*.hpp");
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(t2_2_file_extensions_default)
{
    // Run
    parse({ "" });

    // Check
    BOOST_CHECK_EQUAL(
		toString( resources::arguments::fileExtensions::DefaultValue ),
		toString( getDefaultFileExtensions() )
    );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(t3_1_include_dirs_from_argument)
{
    // Init
    const std::string argumentName = resources::arguments::includeDirs::FullName;
    const std::string argumentValue = "/include/dir1,/include/dir2";

    const std::string argument = "--" + argumentName + "=" + argumentValue;

    // Run
    parse({ argument });

    // Check
    auto includeDirs = getIncludeDirs();
    BOOST_REQUIRE_EQUAL( includeDirs.size(), 2 );
    BOOST_CHECK_EQUAL(includeDirs.at( 0 ), "/include/dir1");
    BOOST_CHECK_EQUAL(includeDirs.at( 1 ), "/include/dir2");
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(t3_2_include_dirs_default)
{
    // Run
    parse({ "" });

    // Check
    BOOST_CHECK_EQUAL(
        toString( resources::arguments::includeDirs::DefaultValue ),
        toString( getDefaultIncludeDirs() )
    );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(t4_1_ignore_dirs_from_argument)
{
    // Init
    const std::string argumentName = resources::arguments::ignoreDirs::FullName;
    const std::string argumentValue = "/include/dir1,/include/dir2";

    const std::string argument = "--" + argumentName + "=" + argumentValue;

    // Run
    parse({ argument });

    // Check
    auto includeDirs = getIgnoreDirs();
    BOOST_REQUIRE_EQUAL( includeDirs.size(), 2 );
    BOOST_CHECK_EQUAL(includeDirs.at( 0 ), "/include/dir1");
    BOOST_CHECK_EQUAL(includeDirs.at( 1 ), "/include/dir2");
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(t4_2_ignore_dirs_default)
{
    // Run
    parse({ "" });

    // Check
    BOOST_CHECK_EQUAL(
        toString( resources::arguments::ignoreDirs::DefaultValue ),
        toString( getDefaultIgnoreDirs() )
    );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(t5_1_configuration_file_from_arguments)
{
    // Init
    const std::string argumentName = resources::arguments::configurationFile::FullName;
	const std::string argumentValue = "./file";

    const std::string argument = "--" + argumentName + "=" + argumentValue;

    // Run
    parse({ argument });

    // Check
    BOOST_CHECK_EQUAL(getConfigurationFile().string(), argumentValue );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(t5_2_configuration_file_default)
{
    // Run
    parse({ "" });

    // Check
    BOOST_CHECK_EQUAL(
        resources::arguments::configurationFile::DefaultValue,
        getDefaultConfigurationFile().string()
    );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(t6_1_help_from_arguments)
{
    // Init
    const std::string argumentName = resources::arguments::help::FullName;
    const std::string argument = "--" + argumentName;

    // Run
    parse({ argument });

    // Check
    BOOST_CHECK( isHelp() );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(t7_verbose)
{
	// Init
	const std::string argumentName = resources::arguments::verbose::FullName;
	const std::string argument = "--" + argumentName;

	// Run
	parse({ argument });

	// Check
	BOOST_CHECK( isVerbose() );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(t8_1_report_from_argument)
{
	// Init
	using namespace resources::arguments;

	const std::string argumentName = report::FullName;
	const std::string argumentValue = report::UnresolvedReport;

	const std::string argument = "--" + argumentName + "=" + argumentValue;

	// Run
	parse({ argument });

	// Check
	BOOST_CHECK_EQUAL(
		toString( getReporterKinds() ),
		toString( reporter::ReporterKind::Unresolved )
	);}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(t8_2_report_default)
{
	// Run
	parse({ "" });

	// Check
	BOOST_CHECK_EQUAL(
		toString( resources::arguments::report::DefaultValue ),
		toString( getDefaultReporterKinds() )
	);
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(t8_3_report_incorrect_name)
{
	// Init
	using namespace resources::arguments;

	const std::string argumentName = report::FullName;
	const std::string argumentValue = "incorrect_name";

	const std::string argument = "--" + argumentName + "=" + argumentValue;

	// Check
	parse({ argument });

	BOOST_CHECK_THROW( getReporterKinds(), IncorrectReport );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(t9_1_report_limit_from_argument)
{
	// Init
	using namespace resources::arguments;

	const std::string argumentName = report_limit::FullName;
	const int argumentValue = 42;

	const std::string argument =
		"--" + argumentName + "=" + std::to_string( argumentValue );

	// Run
	parse({ argument });

	// Check
	BOOST_CHECK_EQUAL( getReportLimit(), argumentValue );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(t9_2_report_limit_default)
{
	// Run
	parse({ "" });

	// Check
	BOOST_CHECK_EQUAL(
		getDefaultReportLimit(),
		resources::arguments::report_limit::DefaultValue
	);
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(t10_1_ignore_system_includes_from_arguments)
{
	// Init
	using namespace resources::arguments;

	const std::string argumentName = ignoreSystemIncludes ::FullName;
	const bool argumentValue = true;

	const std::string argument =
		"--" + argumentName + "=" + ( argumentValue ? "true" : "false" );

	// Run
	parse({ argument });

	// Check
	auto valueOpt = getIgnoreSystemIncludes();
	BOOST_REQUIRE( valueOpt.has_value() );
	BOOST_CHECK_EQUAL( *valueOpt, argumentValue );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(t10_2_ignore_system_includes_default)
{
	// Run
	parse({ "" });

	// Check
	BOOST_CHECK_EQUAL(
		getDefaultIgnoreSystemIncludes(),
		resources::arguments::ignoreSystemIncludes::DefaultValue
	);
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(t11_1_ignore_files_from_argument)
{
	// Init
	using namespace resources::arguments;

	const std::string argumentName = ignoreFiles::FullName;
	const std::string argumentValue = "lib1_*,lib2_*";

	const std::string argument =
		"--" + argumentName + "=" + argumentValue;

	// Run
	parse({ argument });

	// Check

	BOOST_CHECK_EQUAL( getIgnoreFiles(), argumentValue );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(t11_2_ignore_files_default)
{
	// Run
	parse({ "" });

	// Check
	BOOST_CHECK_EQUAL(
		getDefaultIgnoreFiles(),
		toString( resources::arguments::ignoreFiles::DefaultValue )
	);
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(t12_version)
{
	// Init
	const std::string argumentName = resources::arguments::version::FullName;

	const std::string argument = "--" + argumentName;

	// Run
	parse({ argument });

	// Check

	BOOST_CHECK( isVersion() );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(t13_1_report_details_limit_from_argument)
{
	// Init
	using namespace resources::arguments;

	const std::string argumentName = report_details_limit::FullName;
	const int argumentValue = 42;

	const std::string argument =
		"--" + argumentName + "=" + std::to_string( argumentValue );

	// Run
	parse({ argument });

	// Check
	BOOST_CHECK_EQUAL( getReportDetailsLimit(), argumentValue );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(t13_2_report_details_limit_default)
{
	// Run
	parse({ "" });

	// Check
	BOOST_CHECK_EQUAL(
		getDefaultReportDetailsLimit(),
		resources::arguments::report_details_limit::DefaultValue
	);
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(t14_1_analyze_without_extension_from_argument)
{
	// Init
	using namespace resources::arguments;

	const std::string argumentName = analyze_without_extension::FullName;
	const bool argumentValue = true;

	const std::string argument =
		"--" + argumentName + "=" + std::to_string( argumentValue );

	// Run
	parse({ argument });

	// Check
	BOOST_CHECK_EQUAL( getAnalyzeWithoutExtension(), argumentValue );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(t14_2_analyze_without_extension_default)
{
	// Run
	parse({ "" });

	// Check
	BOOST_CHECK_EQUAL(
		getDefaultAnalyzeWithoutExtension(),
		resources::arguments::analyze_without_extension::DefaultValue
	);
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(t15_1_show_std_files_from_argument)
{
	// Init
	using namespace resources::arguments;

	const std::string argumentName = show_std_files::FullName;
	const bool argumentValue = true;

	const std::string argument =
		"--" + argumentName + "=" + std::to_string( argumentValue );

	// Run
	parse({ argument });

	// Check
	auto valueOpt = getShowStdFile();
	BOOST_REQUIRE( valueOpt.has_value() );
	BOOST_CHECK_EQUAL( *valueOpt, argumentValue );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(t15_1_show_std_files_default)
{
	// Run
	parse({ "" });

	// Check
	BOOST_CHECK_EQUAL(
		getDefaultShowStdFile(),
		resources::arguments::show_std_files::DefaultValue
	);
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE_END()

//------------------------------------------------------------------------------

}
