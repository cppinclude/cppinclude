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

------------------------------------------------------------------------------*/

namespace application::test {

//------------------------------------------------------------------------------

BOOST_FIXTURE_TEST_SUITE(ProjectBuildersTets, ProjectBuilderFixture)

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(empty_arguments_and_configuration_file_1)
{
	//Run
	parserArguments( {} );
	buildProject();

	//Check
	BOOST_CHECK_EQUAL( getProjectDir(),		toAbsolutePath("./") );
	BOOST_CHECK_EQUAL( getIncludeDirs(),	toAbsolutePath("./") );
	BOOST_CHECK_EQUAL( getIgnoreDirs(),		"" );
	BOOST_CHECK_EQUAL( getFileExtentions(),	"*.c,*.cpp,*.cxx,*.h,*.hpp,*.hxx" );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(only_arguments_2)
{
	//Init
	using namespace resources::arguments;

	const std::string argPrefix = "--";

	const std::string projectArgName = argPrefix + projectDir::FullName;
	const std::string projectArgValue = "test_project/";

	const std::string includeDirsArgName = argPrefix + includeDirs::FullName;
	const std::string includeDirsArgValue = "lib1/,lib2/";

	const std::string ignoreDirsArgName = argPrefix + ignoreDirs::FullName;
	const std::string ignoreDirsArgValue = "ignore1/,ignore2/";

	const std::string fileExtentionsArgName = argPrefix + fileExtensions::FullName;
	const std::string fileExtentionsArgValue = "*.cpp,*.hpp";

	//Run
	parserArguments( {
		projectArgName,
		projectArgValue,

		includeDirsArgName,
		includeDirsArgValue,

		ignoreDirsArgName,
		ignoreDirsArgValue,

		fileExtentionsArgName,
		fileExtentionsArgValue
	} );
	buildProject();

	//Check
	BOOST_CHECK_EQUAL( getProjectDir(),		toAbsolutePath( projectArgValue ) );
	BOOST_CHECK_EQUAL(
		getIncludeDirs(),
		toAbsolutePath("test_project/lib1/") +
		"," +
		toAbsolutePath("test_project/lib2/")
	);
	BOOST_CHECK_EQUAL(
		getIgnoreDirs(),
		toAbsolutePath("test_project/ignore1/") +
		"," +
		toAbsolutePath("test_project/ignore2/")
	);
	BOOST_CHECK_EQUAL( getFileExtentions(),	fileExtentionsArgValue );

}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(only_configuration_file_3)
{
	//Init
	createDefaultConfigurationFile(
	R"({
		"project_dir" : "./src",
		"file_extensions" : ["*.cpp","*.hpp"],
		"include_dirs" : ["include1","include2"],
		"ignore_dirs" : ["ignore1","ignore2"]
	})"
	);

	//Run
	parserArguments( {} );
	buildProject();

	//Check
	BOOST_CHECK_EQUAL( getProjectDir(),		toAbsolutePath("src/") );
	BOOST_CHECK_EQUAL(
		getIncludeDirs(),
		toAbsolutePath("src/include1/") +
		"," +
		toAbsolutePath("src/include2/")
	);
	BOOST_CHECK_EQUAL(
		getIgnoreDirs(),
		toAbsolutePath("src/ignore1/") +
		"," +
		toAbsolutePath("src/ignore2/")
	);
	BOOST_CHECK_EQUAL( getFileExtentions(),	"*.cpp,*.hpp" );

}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(ignore_system_includes_from_configuration_file_5_1)
{
	//Init
	createDefaultConfigurationFile(
	R"({
		"ignore_system_includes" : true
	})"
	);

	//Run
	parserArguments( {} );
	buildProject();

	//Check
	BOOST_CHECK_EQUAL( getIgnoreSystemIncludes(), true );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(ignore_system_includes_from_arguments_5_2)
{
	//Run
	parserArguments({ "--ignore_system_includes=true" });
	buildProject();

	//Check
	BOOST_CHECK_EQUAL( getIgnoreSystemIncludes(), true );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(ignore_system_includes_default_5_3)
{
	//Run
	parserArguments({});
	buildProject();

	//Check
	BOOST_CHECK_EQUAL(
		getIgnoreSystemIncludes(),
		resources::arguments::ignoreSystemIncludes::DefaultValue
	);
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(ignore_files_from_configuration_file_6_1)
{
	//Init
	createDefaultConfigurationFile(
	R"({
		"ignore_files" : [ "boost.*" ]
	})"
	);

	//Run
	parserArguments( {} );
	buildProject();

	//Check
	BOOST_REQUIRE_EQUAL( getFileFiltersCount(), 1 );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(ignore_files_from_arguments_6_2)
{
	//Run
	parserArguments({ "--ignore_files=boost.*" });
	buildProject();

	//Check
	BOOST_REQUIRE_EQUAL( getFileFiltersCount(), 1 );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(ignore_files_default_6_3)
{
	//Run
	parserArguments({});
	buildProject();

	//Check
	auto defaultValues = toStrings( resources::arguments::ignoreFiles::DefaultValue );

	BOOST_REQUIRE_EQUAL( getFileFiltersCount(), defaultValues.size() );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE_END()

//------------------------------------------------------------------------------

}
