#include "application/test/fixtures/app_test_project_builder_fixture.hpp"

#include "application/resources/app_resources_arguments.hpp"

#include <boost/test/unit_test.hpp>

#include <regex>

/*------------------------------------------------------------------------------

TEST PLAN:

1. Empty arguments and not exist configuration file
2. Only arguments
3. Only configuration file
4. Mix arguments and configuration file
5. Ignore system includes
	5.1 From configuration file
	5.2 From arguments
	5.3 Default
6. Ignore files
	6.1 From configuration file
	6.2 From arguments
	6.3 Default
7. Analyze without extension
	7.1 From Configuration file
	7.2 From arguments
	7.3 Default

------------------------------------------------------------------------------*/

namespace application::test
{
//------------------------------------------------------------------------------

// clazy:excludeall=non-pod-global-static
// NOLINTNEXTLINE(fuchsia-statically-constructed-objects,cert-err58-cpp)
BOOST_FIXTURE_TEST_SUITE( ProjectBuildersTets, ProjectBuilderFixture )

//------------------------------------------------------------------------------

// NOLINTNEXTLINE(fuchsia-statically-constructed-objects,cert-err58-cpp)
BOOST_AUTO_TEST_CASE( t1_empty_arguments_and_configuration_file )
{
	// Run
	parserArguments( {} );
	buildProject();

	// Check
	BOOST_CHECK_EQUAL( getProjectDir(), toAbsolutePath( "./" ) );
	BOOST_CHECK_EQUAL( getIncludeDirs(), toAbsolutePath( "./" ) );
	BOOST_CHECK_EQUAL( getIgnoreDirs(), "" );
	BOOST_CHECK_EQUAL( getFileExtensions(), "*.c,*.cpp,*.cxx,*.h,*.hpp,*.hxx" );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE( t2_only_arguments )
{
	// Init
	using namespace resources::arguments;

	const std::string argPrefix = "--";

	const std::string projectArgName = argPrefix + projectDir::FullName;
	const std::string projectArgValue = "test_project/";

	const std::string includeDirsArgName = argPrefix + includeDirs::FullName;
	const std::string includeDirsArgValue = "lib1/,lib2/";

	const std::string ignoreDirsArgName = argPrefix + ignoreDirs::FullName;
	const std::string ignoreDirsArgValue = "ignore1/,ignore2/";

	const std::string fileExtensionsArgName =
		argPrefix + fileExtensions::FullName;
	const std::string fileExtensionsArgValue = "*.cpp,*.hpp";

	// Run
	parserArguments(
		{ projectArgName, projectArgValue,

		  includeDirsArgName, includeDirsArgValue,

		  ignoreDirsArgName, ignoreDirsArgValue,

		  fileExtensionsArgName, fileExtensionsArgValue } );
	buildProject();

	// Check
	BOOST_CHECK_EQUAL( getProjectDir(), toAbsolutePath( projectArgValue ) );
	BOOST_CHECK_EQUAL(
		getIncludeDirs(), toAbsolutePath( "test_project/lib1/" ) + "," +
							  toAbsolutePath( "test_project/lib2/" ) );
	BOOST_CHECK_EQUAL(
		getIgnoreDirs(), toAbsolutePath( "test_project/ignore1/" ) + "," +
							 toAbsolutePath( "test_project/ignore2/" ) );
	BOOST_CHECK_EQUAL( getFileExtensions(), fileExtensionsArgValue );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE( t3_only_configuration_file )
{
	// Init
	createDefaultConfigurationFile(
		R"({
		"project_dir" : "./src",
		"file_extensions" : ["*.cpp","*.hpp"],
		"include_dirs" : ["include1","include2"],
		"ignore_dirs" : ["ignore1","ignore2"]
	})" );

	// Run
	parserArguments( {} );
	buildProject();

	// Check
	BOOST_CHECK_EQUAL( getProjectDir(), toAbsolutePath( "src/" ) );
	BOOST_CHECK_EQUAL(
		getIncludeDirs(), toAbsolutePath( "src/include1/" ) + "," +
							  toAbsolutePath( "src/include2/" ) );
	BOOST_CHECK_EQUAL(
		getIgnoreDirs(), toAbsolutePath( "src/ignore1/" ) + "," +
							 toAbsolutePath( "src/ignore2/" ) );
	BOOST_CHECK_EQUAL( getFileExtensions(), "*.cpp,*.hpp" );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE( t5_1_ignore_system_includes_from_configuration_file )
{
	// Init
	createDefaultConfigurationFile(
		R"({
		"ignore_system_includes" : true
	})" );

	// Run
	parserArguments( {} );
	buildProject();

	// Check
	BOOST_CHECK_EQUAL( getIgnoreSystemIncludes(), true );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE( t5_2_ignore_system_includes_from_arguments )
{
	// Run
	parserArguments( { "--ignore_system_includes=true" } );
	buildProject();

	// Check
	BOOST_CHECK_EQUAL( getIgnoreSystemIncludes(), true );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE( t5_3_ignore_system_includes_default )
{
	// Run
	parserArguments( {} );
	buildProject();

	// Check
	BOOST_CHECK_EQUAL(
		getIgnoreSystemIncludes(),
		resources::arguments::ignoreSystemIncludes::DefaultValue );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE( t6_1_ignore_files_from_configuration_file )
{
	// Init
	createDefaultConfigurationFile(
		R"({
		"ignore_files" : [ "boost.*" ]
	})" );

	// Run
	parserArguments( {} );
	buildProject();

	// Check
	BOOST_REQUIRE_EQUAL( getFileFiltersCount(), 1 );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE( t6_2_ignore_files_from_arguments )
{
	// Run
	parserArguments( { "--ignore_files=boost.*" } );
	buildProject();

	// Check
	BOOST_REQUIRE_EQUAL( getFileFiltersCount(), 1 );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE( t6_3_ignore_files_default )
{
	// Run
	parserArguments( {} );
	buildProject();

	// Check
	auto defaultValues =
		toStrings( resources::arguments::ignoreFiles::DefaultValues );

	BOOST_REQUIRE_EQUAL( getFileFiltersCount(), defaultValues.size() );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE( t7_1_analyze_without_extension_from_configuration_file )
{
	// Init
	createDefaultConfigurationFile(
		R"({
		"analyze_without_extension" : true
	})" );

	// Run
	parserArguments( {} );
	buildProject();

	// Check
	BOOST_REQUIRE_EQUAL( getAnalyzeWithoutExtension(), true );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE( t7_2_analyze_without_extension_from_arguments )
{
	// Run
	parserArguments( { "--analyze_without_extension=true" } );
	buildProject();

	// Check
	BOOST_REQUIRE_EQUAL( getAnalyzeWithoutExtension(), true );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE( t7_3_analyze_without_extension_default )
{
	// Run
	parserArguments( {} );
	buildProject();

	// Check
	auto defaultValue =
		resources::arguments::analyzeWithoutExtension::DefaultValue;

	BOOST_REQUIRE_EQUAL( getAnalyzeWithoutExtension(), defaultValue );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE_END()

//------------------------------------------------------------------------------

}
