#include "application/test/fixtures/app_test_cmake_project_builder_fixture.hpp"

#include "cmake_project/api/cprj_project.hpp"

#include "application/resources/app_resources_arguments.hpp"

#include <boost/test/unit_test.hpp>

#include <memory>
#include <set>

/*------------------------------------------------------------------------------

TEST PLAN
1. File not exist
2. Default compilation file exist
3. Compilation file from arguments
4. Compilation file from configuration file
5. Include files
	5.1 Several includes
	5.2 Different includes in different files

------------------------------------------------------------------------------*/

namespace application::test {

//------------------------------------------------------------------------------

BOOST_FIXTURE_TEST_SUITE(CMakeProjectBuildersTets, CMakeProjectBuilderFixture)

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(t1_file_not_exist)
{
	//Run
	buildProject();

	//Check
	BOOST_CHECK( !isInitializedProject() );
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(t2_default_compilation_file_exist)
{
	// Init
	addCMakeFile(
		resources::arguments::compileCommands::DefaultValue,
		R"([
		{
		  "directory": "/tmp/project",
		  "command": "/usr/bin/c++ -o file1.cpp.o -c file1.cpp",
		  "file": "file1.cpp"
		},
		{
		  "directory": "/tmp/project",
		  "command": "/usr/bin/c++ -o file2.cpp.o -c file2.cpp",
		  "file": "file2.cpp"
		}
		])"
	);

	//Run
	buildProject();

	//Check
	BOOST_REQUIRE( isInitializedProject() );

	const auto resultFiles = getResultsFiles();
	const auto exceptedFiles = toExceptedFiles( {
		"/tmp/project/file1.cpp",
		"/tmp/project/file2.cpp"
	} );

	BOOST_CHECK_EQUAL_COLLECTIONS(
		resultFiles.begin(),
		resultFiles.end(),
		exceptedFiles.begin(),
		exceptedFiles.end()
	);
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(t3_compilation_file_from_arguments)
{
	// Init
	std::string compilationFile = "build/compile_commands.json";

	addCMakeFile(
		compilationFile,
		R"([
		{
		  "directory": "/tmp/project",
		  "command": "/usr/bin/c++ -o file1.cpp.o -c file1.cpp",
		  "file": "file1.cpp"
		},
		{
		  "directory": "/tmp/project",
		  "command": "/usr/bin/c++ -o file2.cpp.o -c file2.cpp",
		  "file": "file2.cpp"
		}
		])"
	);

	//Run
	parserArgument( "--compile_commands=" + compilationFile );
	buildProject();

	//Check
	BOOST_REQUIRE( isInitializedProject() );

	const auto resultFiles = getResultsFiles();
	const auto exceptedFiles = toExceptedFiles( {
		"/tmp/project/file1.cpp",
		"/tmp/project/file2.cpp"
	} );

	BOOST_CHECK_EQUAL_COLLECTIONS(
		resultFiles.begin(),
		resultFiles.end(),
		exceptedFiles.begin(),
		exceptedFiles.end()
	);
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(t4_compilation_file_from_configuration_file)
{
	//Init
	std::string compilationFile = "build/compile_commands.json";

	addCMakeFile(
		compilationFile,
		R"([
		{
		  "directory": "/tmp/project",
		  "command": "/usr/bin/c++ -o file1.cpp.o -c file1.cpp",
		  "file": "file1.cpp"
		},
		{
		  "directory": "/tmp/project",
		  "command": "/usr/bin/c++ -o file2.cpp.o -c file2.cpp",
		  "file": "file2.cpp"
		}
		])"
	);

	//Run
	setCompilationFileInConfigurationFile( compilationFile );
	buildProject();

	//Check
	BOOST_REQUIRE( isInitializedProject() );

	const auto resultFiles = getResultsFiles();
	const auto exceptedFiles = toExceptedFiles( {
		"/tmp/project/file1.cpp",
		"/tmp/project/file2.cpp"
	} );

	BOOST_CHECK_EQUAL_COLLECTIONS(
		resultFiles.begin(),
		resultFiles.end(),
		exceptedFiles.begin(),
		exceptedFiles.end()
	);
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(t5_1_include_files_several_includes)
{
	//Init
	addCMakeFile(
		resources::arguments::compileCommands::DefaultValue,
		R"([
		{
		  "directory": "/tmp/project",
		  "command": "/usr/bin/c++ -o file1.cpp.o -I./lib1/ -I./lib2/ -c file1.cpp",
		  "file": "file1.cpp"
		}
		])"
	);

	//Run
	buildProject();

	//Check
	BOOST_REQUIRE( isInitializedProject() );

	const auto resultFiles = getResultsFiles();
	const auto exceptedFiles = toExceptedFiles( { "/tmp/project/file1.cpp"	} );

	BOOST_REQUIRE_EQUAL_COLLECTIONS(
		resultFiles.begin(),
		resultFiles.end(),
		exceptedFiles.begin(),
		exceptedFiles.end()
	);

	const auto resultIncludesFile = getResultIncludes( "/tmp/project/file1.cpp" );
	const auto exceptedIncludes = toExceptedFiles( { "./lib1/" , "./lib2/" } );

	BOOST_CHECK_EQUAL_COLLECTIONS(
		resultIncludesFile.begin(),
		resultIncludesFile.end(),
		exceptedIncludes.begin(),
		exceptedIncludes.end()
	);
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_CASE(t5_2_include_files_different_includes_in_different_files)
{
	//Init
	addCMakeFile(
		resources::arguments::compileCommands::DefaultValue,
		R"([
		{
		  "directory": "/tmp/project",
		  "command": "/usr/bin/c++ -o file1.cpp.o -I./lib1/ -c file1.cpp",
		  "file": "file1.cpp"
		},
		{
		  "directory": "/tmp/project",
		  "command": "/usr/bin/c++ -o file2.cpp.o -I./lib2/ -c file2.cpp",
		  "file": "file2.cpp"
		}
		])"
	);

	//Run
	buildProject();

	//Check
	BOOST_REQUIRE( isInitializedProject() );

	const auto resultFiles = getResultsFiles();
	const auto exceptedFiles = toExceptedFiles( {
		"/tmp/project/file1.cpp",
		"/tmp/project/file2.cpp"
	} );

	BOOST_REQUIRE_EQUAL_COLLECTIONS(
		resultFiles.begin(),
		resultFiles.end(),
		exceptedFiles.begin(),
		exceptedFiles.end()
	);

	{
		const auto resultIncludesFile = getResultIncludes( "/tmp/project/file1.cpp" );
		const auto exceptedIncludes = toExceptedFiles( { "./lib1/" } );

		BOOST_CHECK_EQUAL_COLLECTIONS(
			resultIncludesFile.begin(),
			resultIncludesFile.end(),
			exceptedIncludes.begin(),
			exceptedIncludes.end()
		);
	}
	{
		const auto resultIncludesFile = getResultIncludes( "/tmp/project/file2.cpp" );
		const auto exceptedIncludes = toExceptedFiles( { "./lib2/" } );

		BOOST_CHECK_EQUAL_COLLECTIONS(
			resultIncludesFile.begin(),
			resultIncludesFile.end(),
			exceptedIncludes.begin(),
			exceptedIncludes.end()
		);
	}
}

//------------------------------------------------------------------------------

BOOST_AUTO_TEST_SUITE_END()

//------------------------------------------------------------------------------

}

